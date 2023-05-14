/**
 * @typedef {Object} Player
 * @prop {string} name
 * @prop {WebSocket} ws
 */

/**
 * @typedef {Object} Room
 * @prop {string} status
 * @prop {string} name
 * @prop {Player} nowMoving
 * @prop {Player} p0
 * @prop {Player} p1
 * @prop {pipe.ChildProcess} process
 * @prop {Array<string>} board
 */

/**
 * @typedef {Object} HandshakePackage
 * @prop {string} header
 * @prop {string} nickname
 * @prop {boolean} isSingle
 * @prop {string} roomId
 */

/**
 * @typedef {Object} Request
 * @prop {string} type
 * @prop {HandshakePackage} package
 * @prop {string} command
 */

/**
 * @typedef {Object} Status
 * @prop {boolean} success
 * @prop {string} message
 */

/**
 * @typedef {Object} Response
 * @prop {Status} status
 * @prop {Object} content
 */

const express = require('express');
const app = express();
const SocketServer = require('ws').Server;
const pipe=require('child_process');

app.use(express.static(__dirname+"\\www"));

const PORT=1234;
const server=app.listen(PORT);

let wss=new SocketServer({server});
/** @type {Map<string,Room>} */
let roomList=new Map();


wss.on('connection',
/**
 * 
 * @param {WebSocket} ws 
 * @returns {void}
 */
(ws)=>{
    console.log(`web socket connected! (${ws.protocol})`);
    if(ws.protocol!="protocol-chess-game") return ws.close();
    ws.onmessage=(e)=>{
        ws.onmessage=null;
        /** @type {Request} */
        let request=JSON.parse(e.data.toString());
        if(request.type!="handshake") return ws.close();
        let package=request.package;
        console.log(package);
        switch(package.header){
            case "getRoomList":
                getRoomList(ws);
                break;
            case "create":
                createGame(ws,package);
                break;
            case "join":
                joinGame(ws,package);
                break;
        }
    }
})

/**
 * 
 * @param {WebSocket} ws 
 */
function getRoomList(ws){
    if(roomList.size==0) return ws.close(400,"empty-roomList");
    let list=new Array();
    roomList.forEach((room,id)=>{
        list.push({
            id:id,
            status:room.status,
            name:room.name,
            who:room.p0.name
        })
    })
    ws.close(200,JSON.stringify(list));
}

/**
 * 
 * @param {WebSocket} ws 
 * @param {HandshakePackage} package 
 */
function createGame(ws,package){
    let room={
        p0:{ws:ws}
    }
    room.nowMoving=room.p0;
    if(package.isSingle){
        room.p0.name="white";
        room.p1={
            name:"black",
            ws:ws
        };
        room.process=pipe.execFile("application\\chess.exe");
        startGame(room);
    }else{
        ws.on('close',()=>{
            roomList.delete(package.roomId);
        })
        room.p0.name=package.nickname;
        room.status="waiting";
        if(roomList.has(package.roomId)) return ws.close();
        roomList.set(package.roomId,room);
    }
    console.log("created!");
    ws.send(JSON.stringify({status:{success:true}}));
}

/**
 * 
 * @param {WebSocket} ws 
 * @param {HandshakePackage} package 
 */
function joinGame(ws,package){
    if(!roomList.has(package.roomId)) return ws.close(400,"room-not-found");
    let room=roomList.get(package.roomId);
    if(room.status!="waiting") return ws.close(400,"room-is-full");
    room.p0.ws.onmessage=(e)=>{
        e.target.onmessage=null;
        /** @type {Response} */
        let response=JSON.parse(e.data.toString());
        console.log(`receive:${response.type} > ${response.value}`);
        if(response.content=="join-accepted"){
            ws.on('close',()=>{
                roomList.delete(package.roomId);
            })
            room.p1={
                name:package.nickname,
                ws:ws
            }
            room.process=pipe.execFile("application\\chess.exe");
            startGame(roomList.get(package.roomId));
            ws.send(JSON.stringify({status:{success:true}}));
        }
        else ws.close(400,response.content);
    }
    room.p0.ws.send(JSON.stringify({
        type:"join-request",
        package:package
    }));
}

/**
 * 
 * @param {Room} room 
 */
function startGame(room){
    let p0=room.p0;
    let p1=room.p1;
    let process=room.process;

    if(!process) wsClose();

    room.status="playing";
    room.board=[
        'rnbqkbnr',
        'pppppppp',
        '........',
        '........',
        '........',
        '........',
        'PPPPPPPP',
        'RNBQKBNR'
    ];

    let wsMessageCallback=(e)=>{
        /** @type {Request} */
        let request=JSON.parse(e.data.toString());
        if(request.type!="command"){
            e.target.send(JSON.stringify({
                status:{
                    success:false,
                    message:"should-be-command"
                }
            }))
            return;
        }
        if(request.command=="get"){
            e.target.send(JSON.stringify({
                status:{
                    success:true
                },
                content:{
                    status:room.status,
                    who:room.nowMoving===p0?"white":"black",
                    board:room.board
                }
            }))
        }else if(e.target==room.nowMoving.ws){
            console.log(`receive from ${e.target==p0?"white":"black"}:${request.type} > ${request.command}`);
            execute(request.command.toString());
        }else{
            e.target.send(JSON.stringify({
                status:{
                    success:false,
                    message:"not-your-turn"
                }
            }));
        }
    }

    p0.ws.onmessage=wsMessageCallback;
    p1.ws.onmessage=wsMessageCallback;

    process.stdout.on('data',(data)=>{
        console.log("ondata!");
        data=data.toString().replaceAll(' ','.').replaceAll(/[\u0000-\u001F\u007F-\u009F]/g,'').split(';');
        if(data.length!=6) return wsClose();
        let obj={
            status:data[0],
            who:data[1],
            canUndo:data[2]==='1',
            canRedo:data[3]==='1',
            value:data[4],
            board:data[5].match(/.{1,8}/g)
        }
        console.log(`send:${obj.value} > ${obj.status}`);
        room.nowMoving.ws.send(JSON.stringify({
            status:{success:true},
            content:obj
        }));
        room.nowMoving=obj.who=="white"?p0:p1;
        room.status=obj.status;
        room.board=obj.board;
        //if(room.status!="playing") return wsClose();
    })

    let execute=(commands)=>{
        console.log("execute:",commands);
        commands.split(' ').forEach((command)=>process.stdin.write(command+'\n'));
        process.stdin.write("\n");
    }

    let wsClose=()=>{
        console.log("onclose!");
        process.kill();
        p0.close();
        p1.close();
    }
}

console.log("http://localhost:"+PORT);
pipe.exec("start http://localhost:"+PORT);