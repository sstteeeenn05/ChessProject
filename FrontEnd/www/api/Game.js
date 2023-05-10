const WS_URL="ws://localhost:1234";

/**
 * @typedef {Object} Package
 * @prop {string} header
 * @prop {string} ipaddr
 * @prop {string} nickname
 * @prop {boolean} isSingle
 * @prop {string} roomId
 */

export class Game{
    constructor(){
        let ws=new WebSocket(WS_URL,"protocol-chess-game");
        let pckg={
            header:"create",
            ipaddr:"127.0.0.1",
            nickname:"abc",
            isSingle:true,
        }
        new Promise((resolve,reject)=>{
            ws.onopen=()=>{
                ws.onmessage=(e)=>{
                    this.ws=ws;
                    this.roomId=e.data.toString();
                    resolve("Web Socket is ready!");
                }
                ws.onclose=()=>reject("Web Socket is closed!");
                ws.onerror=(event)=>reject(event);
                ws.send(JSON.stringify(pckg));
            }
        })
    }
    sendCommand(command,valueLambda=(value)=>{return value}){
        return new Promise((resolve,reject)=>{
            if(!this.ws) reject("Web Socket is not ready!");
            this.ws.onmessage=(event)=>{
                let data=JSON.parse(event.data.toString());
                data.value=valueLambda(data.value);
                resolve(data);
            }
            this.ws.onclose=()=>reject("Web Socket is closed!");
            this.ws.onerror=(event)=>reject(event);
            this.ws.send(JSON.stringify({
                type:"command",
                value:command
            }));
        })
    }
    getBoard(){
        return this.sendCommand(
            "print",
            (value)=>{return value.match(/.{1,8}/g)}
        )
    }
    promotion(choice){
        return this.sendCommand(choice)
    }
    move(x1,y1,x2,y2){
        return this.sendCommand(`move ${x1} ${y1} ${x2} ${y2}`)
    }
    preview(x,y){
        return this.sendCommand(
            `preview ${x} ${y}`,
            (value)=>{
                return value.split("").map(char=>{return parseInt(char)})
            }
        )
    }
}