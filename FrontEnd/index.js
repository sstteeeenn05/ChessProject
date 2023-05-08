const express = require('express');
const app = express();
const SocketServer = require('ws').Server;
const pipe=require('child_process');

app.use(express.static(__dirname+"\\www"));

const PORT=1234;
const server=app.listen(PORT);

let wss=new SocketServer({server});

class Buffer{
    value="";
    writeable=true;
    clear=()=>{this.value=""};
    isEmpty=()=>{return !this.value.length}
    push=(str)=>{this.value=this.value+str}
}

wss.on('connection',(ws)=>{
    console.log("web socket loaded!");
    let process=pipe.execFile("application\\chess.exe");
    let queue=new Array();
    let buffer=new Buffer();

    ws.on('message',(data)=>{
        console.log("receive command:"+data);
        queue.push(data.toString());
    })

    let sendToClient=(value)=>{
        console.log("send result:\n"+value);
        ws.send(value);
        buffer.writeable=true;
        buffer.clear();
    }
    process.stdout.on('data',(data)=>{
        let result=data.toString();
        console.log("ondata!");
        if(buffer.isEmpty()){
            if(result.indexOf('{')==-1) return sendToClient(result);
            else result=result.substring(result.indexOf('{')+3);
        }
        buffer.push(result);
        if(result.indexOf('}')!=-1)
            sendToClient(buffer.value.substring(0,buffer.value.indexOf('}')-1));
    })

    let queueTimer=setInterval(()=>{
        if(queue.length && buffer.writeable){
            let commands=queue.shift();
            console.log("executing:",commands);
            buffer.writeable=false;
            commands.split(' ').forEach((command)=>process.stdin.write(command+'\n'));
            process.stdin.write("\n");
        }
    },100)

    ws.on('close',()=>{
        clearInterval(queueTimer);
    })
})

console.log("http://localhost:"+PORT);
pipe.exec("start http://localhost:"+PORT);