const WS_URL="ws://localhost:1234";

export class Game{
    constructor(){
        new WebSocket(WS_URL,"protocol-create-game");
        this.ws=new WebSocket(WS_URL,"protocol-input-command");
    }
    static getObj=(event)=>{
        return JSON.parse(event.data.toString())
    }
    generatePromise(command,dataLambda){
        return new Promise((resolve,reject)=>{
            this.ws.onmessage=(event)=>resolve(dataLambda(Game.getObj(event)));
            this.ws.onerror=(event)=>reject(event);
            this.ws.send(command);
        })
    }
    getGameState(){
        return this.generatePromise(
            "print gameState",
            (data)=>{return data.value}
        )
    }
    getBoard(){
        return this.generatePromise(
            "print board",
            (data)=>{return data.value.split(' ')}
        )
    }
    promotion(choice){
        return this.generatePromise(
            choice,
            (data)=>{return data.value}
        )
    }
    move(x1,y1,x2,y2){
        return this.generatePromise(
            `move ${x1} ${y1} ${x2} ${y2}`,
            (data)=>{return data.value}
        )
    }
    preview(x,y){
        return this.generatePromise(
            `preview ${x} ${y}`,
            (data)=>{
                return data.value.replaceAll(' ','').split("")
                    .map(char=>{return parseInt(char)})
            }
        )
    }
}