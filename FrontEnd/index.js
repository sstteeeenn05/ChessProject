/**
 * @typedef {Object} Player
 * @prop {string} name
 * @prop {WebSocket} ws
 */

/**
 * @typedef {Object} Room
 * @prop {string} type
 * @prop {string} status
 * @prop {Player} nowMoving
 * @prop {Player} p0
 * @prop {Player} p1
 * @prop {pipe.ChildProcess} process
 */

/**
 * @typedef {Object} HandshakePackage
 * @prop {string} header
 * @prop {string} ipaddr
 * @prop {string} nickname
 * @prop {boolean} isSingle
 * @prop {string} roomId
 */

/**
 * @typedef {Object} ClientRequest
 * @prop {string} type
 * @prop {string} value
 */

/**
 * @typedef {Object} Status
 * @prop {boolean} success
 * @prop {string} message
 */

/**
 * @typedef {Object} ServerResponse
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

wss.on('connection',(ws)=>{
    console.log(`web socket connected! (${ws.protocol})`);
    if(ws.protocol!="protocol-chess-game") return ws.close();
    ws.onmessage=(e)=>{
        ws.onmessage=undefined;
        /** @type {HandshakePackage} */
        let package=JSON.parse(e.data.toString());
        switch(package.header){
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
 * @param {HandshakePackage} package 
 */
function createGame(ws,package){
    let room={
        p0:{
            name:package.nickname,
            ws:ws
        }
    }
    room.nowMoving=room.p0;
    if(package.isSingle){
        room.type="single";
        room.p1={
            name:"black",
            ws:ws
        };
        room.process=pipe.execFile("application\\chess.exe");
    }else room.type="multiple"
    room.status="waiting";
    roomList.set("123",room);
    ws.send("123");
    if(package.isSingle) startGame("123");
}

/**
 * 
 * @param {WebSocket} ws 
 * @param {HandshakePackage} package 
 */
function joinGame(ws,package){
    if(room.type!="multiple") return ws.close();
    if(!roomList.has(package.roomId)) return ws.close();
    let room=roomList.get(package.roomId);
    if(room.status!="waiting") return ws.close();
    room.p0.ws.onmessage=(e)=>{
        let data=JSON.parse(e.data.toString());
        console.log(`receive:${data.type} > ${data.value}`);
        if(data.value=="Y"){
            room.p1={
                name:package.nickname,
                ws:ws
            };
            room.process=pipe.execFile("application\\chess.exe");
            startGame(package.roomId);
        }
        else reject();
    }
    room.p0.ws.send(package);
}

function startGame(roomId){
    let room=roomList.get(roomId);
    let p0=room.p0;
    let p1=room.p1;
    let process=room.process;

    if(!process) wsClose();

    room.status="playing";

    let wsMessageCallback=(e)=>{
        /** @type {ClientRequest} */
        let request=JSON.parse(e.data.toString());
        console.log(`receive:${request.type} > ${request.value}`);
        switch(request.type){
            case "getArgs":
                if(request.value=="state"){
                    e.target.send(JSON.stringify({
                        status:{
                            success:true
                        },
                        content:{
                            status:room.status,
                            who:room.nowMoving
                        }
                    }))
                }
                break;
            case "command":
                if(e.target==room.nowMoving.ws){
                    execute(request.value.toString());
                }else{
                    e.target.send(JSON.stringify({
                        status:{
                            success:false,
                            message:"not-your-turn"
                        }
                    }));
                }
                break;
        }
    }

    p0.ws.onmessage=wsMessageCallback;
    p1.ws.onmessage=wsMessageCallback;

    process.stdout.on('data',(data)=>{
        console.log("ondata!");
        data=data.toString().replaceAll(' ','.').replaceAll(/[\u0000-\u001F\u007F-\u009F]/g,'').split(';');
        if(data.length!=5) return wsClose();
        /** @type {ServerResponse} */
        let obj={
            status:data[0],
            who:data[1],
            canUndo:data[2]==='1',
            canRedo:data[3]==='1',
            value:data[4]
        }
        console.log(`send:${obj.value} > ${obj.status}`);
        room.nowMoving.ws.send(JSON.stringify({
            status:{success:true},
            content:obj
        }));
        room.nowMoving=obj.who=="white"?p0:p1;
        room.status=obj.status;
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