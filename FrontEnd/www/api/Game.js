const WS_URL="ws://localhost:1234";

/**
 * @typedef {Object} HandshakePackage
 * @prop {string} header
 * @prop {string} nickname
 * @prop {boolean} isSingle
 * @prop {string} roomId
 */

export class Game{
    isReady=false;
    isStart=false;
    joinRequestQueue=new Array();
    ws=new WebSocket(WS_URL,"protocol-chess-game");
    /**
     * 
     * @param {Object} request 
     * @param {string} responseType
     * @returns {Promise}
     */
    generatePromise(request,responseType){
        return new Promise((resolve,reject)=>{
            if(!this.isReady) reject("Web Socket is not ready!");
            this.ws.onmessage=(e)=>{
                let response=JSON.parse(e.data.toString());
                if(response.type==responseType)
                    switch(responseType){
                        case "room-response":
                            if(request.content.isSingle||request.content.header=="join"){
                                this.isStart=true;
                                resolve(response.content);
                            }
                            this.ws.onmessage=(e)=>{
                                let request=JSON.parse(e.data.toString());
                                if(request.type=="join-request") this.joinRequestQueue.push(request.content);
                            }
                            resolve();
                        case "game-args":
                            resolve(response.content)
                    }
            }
            this.ws.onclose=(e)=>reject(e.reason);
            this.ws.onerror=(event)=>reject(event);
            this.ws.send(JSON.stringify(request));
        })
    }
    /**
     * 
     * @param {HandshakePackage} pckg 
     * @returns {Promise}
     */
    connect(pckg){
        return new Promise((resolve,reject)=>{
            let interval=setInterval(()=>{
                if(this.ws.readyState===1){
                    clearInterval(interval);
                    this.isReady=true;
                    this.generatePromise({
                        type:"handshake",
                        content:pckg
                    },"room-response").then(
                        (p0Name)=>resolve(p0Name),
                        ()=>reject()
                    )
                }
            })
        })
    }
    acceptJoinRequest(who){
        this.isStart=true;
        this.ws.send(JSON.stringify({
            type:"join-response",
            content:{
                who:who,
                canJoin:true
            }
        }));
    }
    rejectJoinRequest(who){
        this.ws.send(JSON.stringify({
            type:"join-response",
            content:{
                who:who,
                canJoin:false
            }
        }));
    }
    sendCommand(command){
        return this.generatePromise({
            type:"command",
            content:command
        },"game-args")
    }
    getArgs(){
        return this.sendCommand("get");
    }
    promotion(choice){
        return this.sendCommand(choice);
    }
    move(x1,y1,x2,y2){
        return this.sendCommand(`move ${x1} ${y1} ${x2} ${y2}`);
    }
    preview(x,y){
        return this.sendCommand( `preview ${x} ${y}`);
    }
    readHistory(option){
        return this.sendCommand(option);
    }
    resign(color){
        return this.generatePromise({
            type:"resign",
            content:color
        })
    }
}