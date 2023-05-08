const WS_URL="ws://localhost:1234";

export class Game{
    constructor(){
        new WebSocket(WS_URL,"protocol-create-game");
        this.ws=new WebSocket(WS_URL,"protocol-input-command");
    }
    printBoard(board){
        new Promise((resolve,reject)=>{
            this.ws.onmessage=(event)=>{
                resolve(JSON.parse(event.data.toString()));
            }
            this.ws.onerror=(event)=>{
                reject(event);
            }
            this.ws.send("print board");
        }).then((resolve)=>{
            board=resolve.value.split(' ');
        }).catch(err=>console.error(err))
    }
    move(x1,y1,x2,y2,board){
        new Promise((resolve,reject)=>{
            this.ws.onmessage=(event)=>{
                let status=JSON.parse(event.data.toString()).value;
                if(status=="success") resolve();
                else reject("failed");
            }
            this.ws.onerror=(event)=>{
                reject(event);
            }
            this.ws.send(`move ${x1} ${y1} ${x2} ${y2}`);
        }).then((resolve)=>{
            console.info(resolve);
            this.printBoard(board);
        }).catch(
            err=>console.error(err)
        )
    }
    preview(x,y,maskBoard){
        new Promise((resolve,reject)=>{
            this.ws.onmessage=(event)=>{
                resolve(JSON.parse(event.data.toString()));
            }
            this.ws.onerror=(event)=>{
                reject(event);
            }
            this.ws.send(`preview ${x} ${y}`);
        }).then((resolve)=>{
            maskBoard=resolve.value.replaceAll(' ','').split("").map((char)=>{
                return char==='1';
            })
            console.log(maskBoard);
        }).catch(
            err=>console.error(err)
        )
    }
}