const WS_URL="ws://localhost:1234";

export class Game{
    constructor(){
        new WebSocket(WS_URL,"protocol-create-game");
        this.ws=new WebSocket(WS_URL,"protocol-input-command");
    }
    generatePromise(command,valueLambda=(value)=>{return value}){
        return new Promise((resolve,reject)=>{
            this.ws.onmessage=(event)=>{
                let data=JSON.parse(event.data.toString());
                resolve({
                    status:data.status,
                    value:valueLambda(data.value)
                })
            }
            this.ws.onerror=(event)=>reject(event);
            this.ws.send(command);
        })
    }
    getBoard(){
        return this.generatePromise(
            "print",
            (value)=>{return value.match(/.{1,8}/g)}
        )
    }
    promotion(choice){
        return this.generatePromise(choice)
    }
    move(x1,y1,x2,y2){
        return this.generatePromise(`move ${x1} ${y1} ${x2} ${y2}`)
    }
    preview(x,y){
        return this.generatePromise(
            `preview ${x} ${y}`,
            (value)=>{
                return value.split("").map(char=>{return parseInt(char)})
            }
        )
    }
}