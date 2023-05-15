/**
 * @typedef {Object} Player
 * @prop {string} name
 * @prop {WebSocket} ws
 */

/**
 * @typedef {Object} Room
 * @prop {string} status
 * @prop {Player} nowMoving
 * @prop {Player} p0
 * @prop {Player} p1
 * @prop {pipe.ChildProcess} process
 * @prop {GameArgs} gameArgs
 * @prop {string} fen
 */

/**
 * @typedef {Object} HandshakePackage
 * @prop {string} header
 * @prop {string} nickname
 * @prop {boolean} isSingle
 * @prop {string} roomId
 * @prop {string} fen
 */

/**
 * @typedef {Object} GameArgs
 * @prop {string} status
 * @prop {string} who
 * @prop {boolean} canUndo
 * @prop {boolean} canRedo
 * @prop {string} value
 * @prop {Array<boolean>} maskBoard
 * @prop {Array<string>} board
 */

/**
 * @typedef {Object} JoinResponse
 * @prop {string} who
 * @prop {boolean} canJoin
 */

/**
 * @typedef {Object} ServerResponse
 * @prop {boolean} success
 * @prop {string} message
 */

/**
 * @typedef {Object} Package
 * @prop {string} type
 * @prop {HandshakePackage|GameArgs|JoinResponse|ServerResponse|string} content
 */

const express = require('express');
const app = express();
const SocketServer = require('ws').Server;
const pipe=require('child_process');

app.use(express.static(__dirname+"\\www",{index:"Lobby.html"}));

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
        /** @type {HandshakePackage} */
        let package=request.content;
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
    if(roomList.size==0) return ws.close(1000,"empty-roomList");
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
    /** @type {Room} */
    let room={
        p0:{ws:ws},
        gameArgs:{}
    }
    room.nowMoving=room.p0;
    if(package.isSingle){
        room.fen=package.fen;
        room.p0.name="white";
        room.p1={
            name:"black",
            ws:ws
        };
        room.process=pipe.spawn("application\\chess.exe",[room.fen]);
        startGame(room);
    }else{
        if(roomList.has(package.roomId)) return ws.close(1000,"Room Existed");
        ws.on('close',()=>{
            roomList.delete(package.roomId);
        })
        room.p0.name=package.nickname;
        room.status="waiting";
        roomList.set(package.roomId,room);
    }
    console.log("created!");
    ws.send(JSON.stringify({
        type:"room-response",
        content:{success:true}
    }))
}

/**
 * 
 * @param {WebSocket} ws 
 * @param {HandshakePackage} package 
 */
function joinGame(ws,package){
    if(!roomList.has(package.roomId)) return ws.close(1000,"404 NOT FOUND");
    let room=roomList.get(package.roomId);
    if(room.status!="waiting") return ws.close(1000,"The game started");
    room.p0.ws.addEventListener('message',(e)=>{
        /** @type {Package} */
        let response=JSON.parse(e.data.toString());
        if(response.type!="join-response") return;
        /** @type {JoinResponse} */
        let joinResponse=response.content;
        console.log(`receive:${joinResponse.who} > ${joinResponse.canJoin}`);
        if(joinResponse.who==package.nickname){
            if(joinResponse.canJoin){
                ws.on('close',()=>{
                    roomList.delete(package.roomId);
                })
                room.p1={
                    name:package.nickname,
                    ws:ws
                }
                room.process=pipe.execFile("application\\chess.exe");
                startGame(roomList.get(package.roomId));
                ws.send(JSON.stringify({
                    type:"room-response",
                    content:{success:true}
                }))
            }
            else{
                console.log(package.nickname+"closed");
                ws.close(1000,"join-rejected");
            }
        }
    })
    room.p0.ws.send(JSON.stringify({
        type:"join-request",
        content:package
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
    let gameArgs=room.gameArgs;

    if(!process) wsClose();

    room.status="playing";
    gameArgs={
        status:"playing",
        who:"white",
        canUndo:false,
        canRedo:false,
        value:"",
        maskBoard:[],
        board:[
            'rnbqkbnr',
            'pppppppp',
            '........',
            '........',
            '........',
            '........',
            'PPPPPPPP',
            'RNBQKBNR'
        ]
    }

    let wsMessageCallback=(e)=>{
        /** @type {Request} */
        let request=JSON.parse(e.data.toString());
        if(request.type!="command"){
            e.target.send(JSON.stringify({
                type:"server-response",
                content:{
                    success:false,
                    message:"should-be-command"
                }
            }))
            return;
        }
        if(request.content=="get"){
            e.target.send(JSON.stringify({
                type:"game-args",
                content:gameArgs
            }))
        }else if(e.target==room.nowMoving.ws){
            console.log(`receive from ${e.target==p0?"white":"black"}:${request.type} > ${request.content}`);
            execute(request.content.toString());
        }else{
            e.target.send(JSON.stringify({
                type:"server-response",
                content:{
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
        let arr=data.toString().replaceAll(' ','.').replaceAll(/[\u0000-\u001F\u007F-\u009F]/g,'').split(';');
        if(arr.length!=7) return wsClose("Internal Server Error");
        gameArgs={
            status:arr[0],
            who:arr[1],
            canUndo:arr[2]==='1',
            canRedo:arr[3]==='1',
            value:arr[4],
            maskBoard:arr[5].toString().split("").map((c)=>{return parseInt(c)}),
            board:arr[6].match(/.{1,8}/g)
        }
        console.log(`send:${gameArgs.value}${gameArgs.maskBoard} > ${gameArgs.status}`);
        room.nowMoving.ws.send(JSON.stringify({
            type:"game-args",
            content:gameArgs
        }));
        room.nowMoving=gameArgs.who=="white"?p0:p1;
        //if(room.status!="playing") return wsClose();
    })

    let execute=(commands)=>{
        console.log("execute:",commands);
        commands.split(' ').forEach((command)=>process.stdin.write(command+'\n'));
        process.stdin.write("\n");
    }

    p0.ws.onclose=()=>{
        console.log("player0 leave!");
        if(p1.ws.readyState===1) p1.ws.close(1000,"Player0 leave!");
    }
    p1.ws.onclose=()=>{
        console.log("player1 leave!");
        if(p0.ws.readyState===1) p0.ws.close(1000,"Player1 leave!");
    }

    let wsClose=(msg)=>{
        console.log("onclose!");
        p0.ws.onclose=null;
        p1.ws.onclose=null;
        p0.ws.close(1000,msg);
        p1.ws.close(1000,msg);
        process.write("exit\n");
    }
}

console.log("http://localhost:"+PORT);
pipe.exec("start http://localhost:"+PORT);