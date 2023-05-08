const WS_URL="ws://localhost:1234";

export class Game{
    constructor(){
        new WebSocket(WS_URL,"protocol-create-game");
        this.ws=new WebSocket(WS_URL,"protocol-input-command");
    }
    printBoard(){
        return new Promise((resolve,reject)=>{
            this.ws.onmessage=(event)=>{
                resolve(JSON.parse(event.data.toString()).value.split(' '));
            }
            this.ws.onerror=(event)=>{
                reject(event);
            }
            this.ws.send("print board");
        })
    }
    move(x1,y1,x2,y2){
        return new Promise((resolve,reject)=>{
            this.ws.onmessage=(event)=>{
                let status=JSON.parse(event.data.toString()).value;
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
                resolve(JSON.parse(event.data.toString()).value
                    .replaceAll(' ','')
                    .split("").map(char=>{return char==='1';})
                );
            }
            this.ws.onerror=(event)=>{
                reject(event);
            }
            this.ws.send(`preview ${x} ${y}`);
        })
    }
}