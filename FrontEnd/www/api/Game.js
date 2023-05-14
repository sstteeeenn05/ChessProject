const WS_URL="ws://localhost:1234";

/**
 * @typedef {Object} HandshakePackage
 * @prop {string} header
 * @prop {string} nickname
 * @prop {boolean} isSingle
 * @prop {string} roomId
 */

export class Game{
    isStart=false;
    joinRequestQueue=new Array();
    /**
     * 
     * @param {HandshakePackage} pckg 
     * @returns {Promise}
     */
    connect(pckg){
        return new Promise((resolve,reject)=>{
            let ws=new WebSocket(WS_URL,"protocol-chess-game");
            ws.onopen=()=>{
                ws.onmessage=()=>{
                    this.ws=ws;
                    if(pckg.isSingle||pckg.header=="join"){
                        this.isStart=true;
                        resolve();
                    }
                    ws.onmessage=(e)=>{
                        let request=JSON.parse(e.data.toString());
                        if(request.type=="join-request") this.joinRequestQueue.push(request.package);
                    }
                    resolve();
                }
                ws.onclose=(e)=>reject(e.reason);
                ws.onerror=(event)=>reject(event);
                ws.send(JSON.stringify({
                    type:"handshake",
                    package:pckg
                }));
            }
        })
    }
    /**
     * 
     * @param {*} request 
     * @param {?(string)=>string|string[]|int[]} valueLambda 
     * @returns {Promise}
     */
    generatePromise(request,valueLambda){
        return new Promise((resolve,reject)=>{
            if(!this.ws) reject("Web Socket is not ready!");
            this.ws.onmessage=(event)=>{
                let response=JSON.parse(event.data.toString());
                if(!response.status.success) reject(response.status.message);
                if(valueLambda) response.content.value=valueLambda(response.content.value);
                resolve(response.content);
            }
            this.ws.send(JSON.stringify(request));
        })
    }
    acceptJoinRequest(){
        this.isStart=true;
        this.ws.send(JSON.stringify({
            type:"join-response",
            content:"join-accepted"
        }));
    }
    rejectJoinRequest(){
        this.ws.send(JSON.stringify({
            type:"join-response",
            content:"join-rejected"
        }));
    }
    sendCommand(command,valueLambda=(value)=>{return value}){
        return this.generatePromise({
            type:"command",
            command:command
        },valueLambda)
    }
    getState(){
        return this.sendCommand("get");
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
    readHistory(option){
        return this.sendCommand(option);
    }
}