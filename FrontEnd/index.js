/**
 * @typedef {Object} Player
 * @prop {string} name
 * @prop {WebSocket} ws
 * @prop {number} remainTime
 * @prop {boolean} timerPaused
 */

/**
 * @typedef {Object} Room
 * @prop {string} status
 * @prop {Player} nowMoving
 * @prop {Player} p0
 * @prop {Player} p1
 * @prop {pipe.ChildProcess} process
 * @prop {Array<string>} commandList
 * @prop {GameArgs} gameArgs
 * @prop {string} fen
 * @prop {number} time
 * @prop {number} addPerRound
 */

/**
 * @typedef {Object} HandshakePackage
 * @prop {string} header
 * @prop {string} nickname
 * @prop {boolean} isSingle
 * @prop {string} roomId
 * @prop {string} fen
 * @prop {number} time
 * @prop {number} addPerRound
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
 * @prop {Player} p0
 * @prop {Player} p1
 * @prop {number} time
 * @prop {number} addPerRound
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
    if(ws.protocol!="protocol-chess-game") return ws.close(1002);
    ws.onmessage=(e)=>{
        ws.onmessage=null;
        /** @type {Request} */
        let request=JSON.parse(e.data.toString());
        if(request.type!="handshake") return ws.close(1003);
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
    let list=new Array();
    roomList.forEach((room,id)=>{
        list.push({
            id:id,
            status:room.status,
            p0:room.p0.name,
            p1:room.p1?room.p1.name:undefined
        })
    })
    list.sort((a,b)=>{
        if(a.status==b.status) return 0;
        if(a.status=="Waiting"&&b.status!="Waiting") return 1;
        if(a.status!="Waiting"&&b.status=="Waiting") return -1;
    })
    list.reverse();
    ws.send(JSON.stringify({
        type:"room-list",
        content:list
    }));
}

/**
 * 
 * @param {WebSocket} ws 
 * @param {HandshakePackage} package 
 */
function createGame(ws,package){
    /** @type {Room} */
    let room={
        p0:{
            ws:ws,
            remainTime:package.time,
            timerPaused:true
        },
        p1:{
            remainTime:package.time,
            timerPaused:true
        },
        gameArgs:{},
        commandList:[],
        time:package.time,
        addPerRound:package.addPerRound,
    }
    room.nowMoving=room.p0;
    if(package.isSingle){
        room.fen=package.fen;
        room.p0.name="white";
        room.p1.name="black";
        room.p1.ws=ws;
        room.process=pipe.exec(`application\\chess.exe ${room.fen}`);
        startGame(room);
    }else{
        if(roomList.has(package.roomId)) return ws.close(1003,"Room Existed");
        ws.on('close',()=>{
            roomList.delete(package.roomId);
        })
        room.p0.name=package.nickname;
        room.status="Waiting";
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
    if(!roomList.has(package.roomId)) return ws.close(1003,"404 ROOM NOT FOUND");
    let room=roomList.get(package.roomId);
    if(room.status!="Waiting") return ws.close(1003,"The game started");
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
                room.p1.name=package.nickname;
                room.p1.ws=ws;
                room.process=pipe.execFile("application\\chess.exe");
                startGame(roomList.get(package.roomId));
                ws.send(JSON.stringify({
                    type:"room-response",
                    content:{success:true}
                }))
            }
            else{
                console.log(package.nickname+"closed");
                ws.close(1003,"join-rejected");
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

    room.status="Playing";

    let p0Timer=setInterval(()=>{
        if(!p0.timerPaused){
            p0.remainTime--;
            if(p0.remainTime<=0){
                if(p0==p1) wsClose(`Time's up! Black Wins`);
                else wsClose(`Time's up! ${p1.name} Wins`, `Time's up! You Win`);
            }
        }
    },1000)
    let p1Timer=setInterval(()=>{
        if(!p1.timerPaused){
            p1.remainTime--;
            if(p1.remainTime<=0){
                if(p0==p1) wsClose(`Time's up! White Wins`);
                else wsClose(`Time's up! You Win`, `Time's up! ${p0.name} Wins`);
            }
        }
    },1000)

    function updateGameArgs(){
        gameArgs.p0=p0;
        gameArgs.p1=p1;
    }

    function checkCommandToPauseTimer(value){
        if(room.commandList.length==1){
            p0.timerPaused=false;
            return;
        }
        let lastCommand=room.commandList[room.commandList.length-1];
        if(lastCommand=="move"&&value=="success"){
            room.nowMoving.timerPaused=true;
            let nextMoving=room.nowMoving==p0?p1:p0;
            nextMoving.remainTime=Math.min(room.time,nextMoving.remainTime+room.addPerRound);
            nextMoving.timerPaused=false;
        }
    }

    let wsMessageCallback=(e)=>{
        /** @type {Request} */
        let request=JSON.parse(e.data.toString());
        if(request.type=="resign"){
            if(request.content=="white") wsClose(`${p0.name} Resign! ${p1.name} Wins!`);
            else wsClose(`${p1.name} Resign! ${p0.name} Wins!`);
            return;
        }
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
            updateGameArgs();
            e.target.send(JSON.stringify({
                type:"game-args",
                content:gameArgs
            }))
        }else if(e.target==room.nowMoving.ws){
            console.log(`receive from ${e.target==p0?"white":"black"}:${request.type} > ${request.content}`);
            execute(request.content);
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
            board:arr[6]?arr[6].match(/.{1,8}/g):gameArgs.board
        }
        checkCommandToPauseTimer(gameArgs.value);
        updateGameArgs();
        console.log(`send:${gameArgs.value}${gameArgs.maskBoard} > ${gameArgs.status}`);
        room.nowMoving.ws.send(JSON.stringify({
            type:"game-args",
            content:gameArgs
        }));
        room.nowMoving=gameArgs.who=="white"?p0:p1;
    })

    let execute=(commands)=>{
        if(typeof commands!='number') room.commandList.push(commands.split(' ')[0]);
        commands=commands.toString();
        console.log("execute:",commands);
        commands.split(' ').forEach((command)=>process.stdin.write(command+'\n'));
        process.stdin.write("\n");
    }

    p0.ws.onclose=()=>{
        console.log("Player0 quit!");
        if(p1.ws.readyState===1) p1.ws.close(1000,`${p0.name} quit the game!`);
    }
    p1.ws.onclose=()=>{
        console.log("Player1 quit!");
        if(p0.ws.readyState===1) p0.ws.close(1000,`${p1.name} quit the game!`);
    }

    let wsClose=(msg0,msg1=msg0)=>{
        console.log("onclose!");
        clearInterval(p0Timer);
        clearInterval(p1Timer);
        p0.ws.onclose=null;
        p1.ws.onclose=null;
        p0.ws.close(1000,msg0);
        p1.ws.close(1000,msg1);
        process.stdin.write("exit\n");
    }

    process.stdin.write("start\n");
}

console.log("http://localhost:"+PORT);
pipe.exec("start http://localhost:"+PORT);