const express = require('express');
const app = express();
const SocketServer = require('ws').Server;
const pipe=require('child_process');

app.use(express.static(__dirname+"\\www"));

const PORT=1234;
const server=app.listen(PORT);

let wss=new SocketServer({server});

wss.on('connection',(ws)=>{
    console.log("web socket loaded!");
    let process=pipe.exec("application\\chess.exe");
    ws.on('message',(data)=>{
        console.log("receive msg:"+data);
        process.stdin.write(data+'\n');
    })
    process.stdout.on('data',(data)=>{
        console.log("send msg:"+data);
        ws.send(data);
    })
})

console.log("http://localhost:"+PORT);
pipe.exec("start http://localhost:"+PORT);