const express = require('express');
const app = express();
const SocketServer = require('ws').Server;
const pipe=require('child_process');

app.use(express.static(__dirname+"\\www"));

const PORT=1234;
const server=app.listen(PORT);

/**
 * @typedef {Object} Room
 * @prop {string} type
 * @prop {string} status
 * @prop {WebSocket} nowMoving
 * @prop {WebScoket} p0
 * @prop {WebScoket} p1
 * @prop {pipe.ChildProcess} process
 * @prop {Array} queue
 */

let wss=new SocketServer({server});
/** @type {Map<string,Room>} */
let roomList=new Map();

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
    let room={p0:ws,nowMoving:ws,queue:new Array()}
    if(package.isSingle){
        room.type="single";
        room.p1=ws;
        room.process=pipe.execFile("application\\chess.exe");
    }else room.type="multiple"
    room.status="waiting";
    roomList.set("123",room);
    ws.send("123");
    if(package.isSingle) startGame("123");
}

function joinGame(ws,package){
    if(room.type!="multiple") return ws.close();
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
        room.p1=ws;
        room.process=pipe.execFile("application\\chess.exe");
        startGame(package.roomId);
    })
}

function startGame(roomId){
    let room=roomList.get(roomId);
    let queue=room.queue;
    let process=room.process;

    if(!process) wsClose();

    room.status="playing";

    let wsMessageCallback=(e)=>{
        let data=JSON.parse(e.data.toString());
        console.log(`receive:${data.type} > ${data.value}`);
        switch(data.type){
            case "getState":
                e.target.send(JSON.stringify({
                    status:room.status,
                    who:room.nowMoving
                }))
                break;
            case "command":
                console.log("push to queue");
                queue.push({
                    who:e.target,
                    value:data.value
                });
                break;
        }
    }

    room.p0.onmessage=wsMessageCallback;
    room.p1.onmessage=wsMessageCallback;

    process.stdout.on('data',(data)=>{
        console.log("ondata!");
        data=data.toString().replaceAll(' ','.').replaceAll(/[\u0000-\u001F\u007F-\u009F]/g,'').split(';');
        if(data.length!=5) return wsClose();
        let obj={
            status:data[0],
            who:data[1],
            canUndo:data[2]==='1',
            canRedo:data[3]==='1',
            value:data[4]
        }
        console.log(`send:${obj.value} > ${obj.status}`);
        room.nowMoving.send(JSON.stringify(obj));
        room.nowMoving=obj.who=="white"?room.p0:room.p1;
        room.status=obj.status;
        //if(room.status!="playing") return wsClose();
    })

    let queueTimer=setInterval(()=>{
        if(queue.length&&queue.at(0).who==room.nowMoving){
            let commands=queue.shift().value.toString();
            console.log("execute:",commands);
            commands.split(' ').forEach((command)=>process.stdin.write(command+'\n'));
            process.stdin.write("\n");
        }
    },20)

    let wsClose=()=>{
        console.log("onclose!");
        clearInterval(queueTimer);
        process.kill();
        room.p0.close();
        room.p1.close();
    }
}

console.log("http://localhost:"+PORT);
pipe.exec("start http://localhost:"+PORT);