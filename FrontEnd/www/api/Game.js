const WS_URL="ws://localhost:1234";

export class Game{
    constructor(){
        new WebSocket(WS_URL,"protocol-create-game");
        this.ws=new WebSocket(WS_URL,"protocol-input-command");
    }
    static getObj=(event)=>{
        return JSON.parse(event.data.toString())
    }
    getGameState(){
        return new Promise((resolve,reject)=>{
            this.ws.onmessage=(event)=>{
                resolve(Game.getObj(event).value);
            }
            this.ws.onerror=(event)=>{
                reject(event);
            }
            this.ws.send("print gameState")
        })
    }
    getBoard(){
        return new Promise((resolve,reject)=>{
            this.ws.onmessage=(event)=>{
                resolve(Game.getObj(event).value.split(' '));
            }
            this.ws.onerror=(event)=>{
                reject(event);
            }
            this.ws.send("print board");
        })
    }
    promotion(choice){
        return new Promise((resolve,reject)=>{
            this.ws.onmessage=(event)=>{
                resolve(Game.getObj(event).value);
            }
            this.ws.onerror=(event)=>{
                reject(event);
            }
            this.ws.send(choice);
        })
    }
    move(x1,y1,x2,y2){
        return new Promise((resolve,reject)=>{
            this.ws.onmessage=(event)=>{
                let status=Game.getObj(event).value;
                if(status=="failed") reject("failed");
                else resolve(status);
            }
            this.ws.onerror=(event)=>{
                reject(event);
            }
            this.ws.send(`move ${x1} ${y1} ${x2} ${y2}`);
        })
    }
    preview(x,y){
        return new Promise((resolve,reject)=>{
            this.ws.onmessage=(event)=>{
                resolve(Game.getObj(event).value
                    .replaceAll(' ','')
                    .split("").map(char=>{return 0+(char==='1');})
                );
            }
            this.ws.onerror=(event)=>{
                reject(event);
            }
            this.ws.send(`preview ${x} ${y}`);
        })
    }
}