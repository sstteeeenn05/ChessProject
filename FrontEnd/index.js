const express = require('express');
const app = express();
const SocketServer = require('ws').Server;
const pipe=require('child_process');

app.use(express.static(__dirname+"\\www"));

const PORT=1234;
const server=app.listen(PORT);

/**
 * @typedef {Object} Room
 * @prop {string} status
 * @prop {string} nowMoving
 * @prop {WebScoket} p0
 * @prop {WebScoket} p1
 * @prop {pipe.ChildProcess} process
 * @prop {Array} queue
 */

let wss=new SocketServer({server});
/** @type {Map<string,Room>} */
let roomList=new Map();

/** @type {pipe.ChildProcess} */
let process;

/**
 * @typedef {Object} Package
 * @prop {string} header
 * @prop {string} ipaddr
 * @prop {string} nickname
 * @prop {boolean} isSingle
 * @prop {string} roomId
 */

wss.on('connection',(ws)=>{
    console.log(`web socket connected! (${ws.protocol})`);
    if(ws.protocol!="protocol-chess-game") return ws.close();
    ws.onmessage=(e)=>{
        ws.onmessage=undefined;
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

function createGame(ws,package){
    let room={p0:ws,nowMoving:"white",queue:new Array()}
    initGame(ws,room.queue);
    if(package.isSingle){
        room.status="single";
        room.p1=ws;
        room.process=pipe.execFile("application\\chess.exe");
    }else room.status="waiting";
    roomList.set("123",room);
    ws.send("123");
    if(package.isSingle) startGame("123");
}

function joinGame(ws,package){
    if(!roomList.has(package.roomId)) return ws.close();
    let room=roomList.get(package.roomId);
    if(room.status!="waiting") return ws.close();
    new Promise((resolve,reject)=>{
        room.p0.onmessage=(e)=>{
            let data=JSON.parse(e.data.toString());
            console.log(`receive:${data.type} > ${data.value}`);
            if(data.value=="Y") resolve();
            else reject();
        }
        room.p0.send(package);
    }).then(()=>{
        room.status="full";
        initGame(ws,room.queue);
        room.p1=ws;
        room.process=pipe.execFile("application\\chess.exe");
        startGame(package.roomId);
    })
}

function initGame(ws,queue){
    ws.onmessage=(e)=>{
        let data=JSON.parse(e.data.toString());
        console.log(`receive:${data.type} > ${data.value}`);
        switch(data.type){
            case "command":
                console.log("push to queue");
                queue.push(data.value);
                break;
        }
    }
}

function startGame(roomId){
    let room=roomList.get(roomId);
    let queue=room.queue;
    let process=room.process;

    if(!process){
        room.p0.close();
        room.p1.close();
    }

    process.stdout.on('data',(data)=>{
        console.log("ondata!");
        data=data.toString().replaceAll(' ','.').replaceAll(/[\u0000-\u001F\u007F-\u009F]/g,'').split(';');
        if(data.length!=4){
            room.p0.close();
            room.p1.close();
            return;
        }
        let obj={
            status:data[0],
            canUndo:data[1]==='1',
            canRedo:data[2]==='1',
            value:data[3]
        }
        //TODO: tell ws2 it's now your turn and deal with win/lose/tie
        let ws1=room.nowMoving=='white'?room.p0:room.p1;
        room.nowMoving=obj.status.split('.')[0];
        let ws2=room.nowMoving=='white'?room.p0:room.p1;
        console.log(`send:${obj.value} > ${obj.status}`);
        ws1.send(JSON.stringify(obj));
    })

    let queueTimer=setInterval(()=>{
        if(queue.length){
            let commands=queue.shift().toString();
            console.log("execute:",commands);
            commands.split(' ').forEach((command)=>process.stdin.write(command+'\n'));
            process.stdin.write("\n");
        }
    },20)

    let onclose=()=>{
        console.log("onclose!");
        clearInterval(queueTimer);
    }
    room.p0.onclose=onclose;
    room.p1.onclose=onclose;
}

console.log("http://localhost:"+PORT);
pipe.exec("start http://localhost:"+PORT);