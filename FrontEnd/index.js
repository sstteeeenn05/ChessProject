const express = require('express');
const app = express();
const SocketServer = require('ws').Server;
const pipe=require('child_process');

app.use(express.static(__dirname+"\\www"));

const PORT=1234;
const server=app.listen(PORT);

/**
 * @typedef {Object} Room
 * @prop {WebScoket} p0
 * @prop {WebScoket} p1
 * @prop {pipe.ChildProcess} process
 */

let wss=new SocketServer({server});
/** @type {Map<number,Room>} */
let room=new Map();

let process;

/**
 * @typedef {Object} CreateArgs
 * @prop {boolean} isSingle
 * @prop {string} ipaddr
 * @prop {string} nickname
 */

/**
 * @typedef {Object} JoinArgs
 * @prop {number} roomId
 * @prop {string} ipaddr
 * @prop {string} nickname
 */

/**
 * @typedef {Object} Package
 * @prop {string} header
 * @prop {CreateArgs|JoinArgs} args
 */

wss.on('connection',(ws)=>{
    console.log(`web socket connected! (${ws.protocol})`);
    switch(ws.protocol){
        case "protocol-create-game":
            createGame(ws);
            break;
        case "protocol-input-command":
            inputCommand(ws);
            break;
    }
})

function createGame(ws){
    console.log(ws.url);
    process=pipe.execFile("application\\chess.exe");
}

function inputCommand(ws){
    let queue=new Array();

    if(!process) ws.close();

    ws.on('message',(data)=>{
        console.log("receive:"+data);
        queue.push(data.toString());
    })

    process.stdout.on('data',(data)=>{
        console.log("ondata!");
        data=data.toString().replaceAll(' ','.').replaceAll(/[\u0000-\u001F\u007F-\u009F]/g,'').split(';');
        console.log(data.length)
        if(data.length!=2) ws.close();
        else{
            let obj={status:data[0],value:data[1]}
            console.log("status:"+obj.status);
            console.log("send:"+obj.value);
            ws.send(JSON.stringify(obj));
        }
    })

    let queueTimer=setInterval(()=>{
        if(queue.length){
            let commands=queue.shift();
            console.log("execute:",commands);
            commands.split(' ').forEach((command)=>process.stdin.write(command+'\n'));
            process.stdin.write("\n");
        }
    },20)

    ws.on('close',()=>{
        console.log("onclose!");
        clearInterval(queueTimer);
    })
}

console.log("http://localhost:"+PORT);
pipe.exec("start http://localhost:"+PORT);