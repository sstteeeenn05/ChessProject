export class Game{
    pending=false;
    buffer;
    constructor(){
        this.ws=new WebSocket("ws://localhost:1234");
        this.ws.onmessage((event)=>{
            this.buffer=event.data.toString().split('\n');
            this.pending=false;
        })
    }
    move(x1,y1,x2,y2){
        if(this.pending) return alert("fuckyou");
        this.pending=true;
        this.ws.send(`move ${x1} ${y1} ${x2} ${y2}`);
        while(this.pending);
        return this.buffer;
    }
}