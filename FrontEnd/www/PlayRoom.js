import "./api/Alpine.js"
import {Game} from "./api/Game.js"

function getUrlQuery(key){
    let query=location.search;
    let arr=query.split(key);
    if(arr.length<2) return false;
    return decodeURI(arr[1].replace('=','').split('&')[0]);
}

document.addEventListener('alpine:init',()=>{
    Alpine.store('utility',{
        isupper(input){
            return (input === input.toUpperCase() && input !== '.');
        },
        islower(input){
            return (input === input.toLowerCase() && input !== '.');
        },
        calculateUrl(input) {
            if (input === 'z') return './assets/joy.png';
            if (input !== '.') {
                if (this.islower(input))
                    return './assets/b' + input + '.svg';
                return './assets/w' + input.toLowerCase() + '.svg';
            }
            return './assets/nothing.png';
        }
    })
    Alpine.store('game',{
        api:new Game(),
        roundMessage:"",
        isSingle:false,
        playerColor:"",
        nowMovingColor:"white",
        p0:{
            name:"",
            remainTime:0
        },
        p1:{
            name:"",
            remainTime:0
        },
        args:{
            status:"playing",
            color:"white",
            canUndo:false,
            canRedo:false,
            value:"",
            maskBoard:[],
            board:[]
        },
        updateInterval:null,
        update(resolve){
            this.args=resolve.args;
            this.p0.remainTime=resolve.p0RemainTime;
            this.p1.remainTime=resolve.p1RemainTime;
        },
        startUpdate(){
            this.updateInterval=setInterval(()=>{
                this.api.getArgs().then((resolve)=>{
                    this.update(resolve);
                    if(this.playerColor===this.args.color
                        &&this.playerColor!==this.nowMovingColor
                        &&!this.isSingle)
                        this.changeTurn(resolve.args);
                })
            },100)
        },
        stopUpdate(){
            clearInterval(this.updateInterval);
        },
        connect(){
            let pckg=new Object();

            if(getUrlQuery("create")!==false){
                this.playerColor="white";
                pckg.header="create";
                if(getUrlQuery("clock")===false){
                    Alpine.store('loading').message="missing header";
                    return;
                }
                pckg.time=parseInt(getUrlQuery("clock").split('+')[0])*60;
                pckg.addPerRound=parseInt(getUrlQuery("clock").split('+')[1]);
                if(getUrlQuery("single")!==false){
                    pckg.isSingle=true;
                    this.isSingle=true;
                    if(getUrlQuery("fen")!==false) pckg.fen=getUrlQuery("fen");
                }else{
                    pckg.isSingle=false;
                    pckg.roomId=getUrlQuery("create");
                    this.p0.name=getUrlQuery("nickname")!==false?getUrlQuery("nickname"):"Unknown";
                    pckg.nickname=this.p0.name;
                    Alpine.store('loading').message="Wating for Player's Joining";
                }
            }else if(getUrlQuery("join")!==false){
                this.playerColor="black";
                pckg.header="join";
                pckg.roomId=getUrlQuery("join");
                this.p1.name=getUrlQuery("nickname")!==false?getUrlQuery("nickname"):"Unknown";
                pckg.nickname=this.p1.name;
                Alpine.store('loading').message="Wating for Room Leader's Reply";
            }else{
                Alpine.store('loading').message="missing header";
                return;
            }

            Alpine.store('onClose').add();
            this.api.connect(pckg).then((resolve)=>{
                if(pckg.header=="join") this.p0.name=resolve;
                this.waitJoin();
            }).catch((msg)=>Alpine.store('loading').message=msg);
        },
        waitJoin(){
            let interval=setInterval(()=>{
                if(this.api.isStart){
                    clearInterval(interval);
                    Alpine.store('music').play('bgmHead');
                    Alpine.store('loading').message="Joining";
                    this.startUpdate();
                    setTimeout(()=>{
                        this.changeTurn(this.args);
                        Alpine.store('loading').on=false;
                        Alpine.store('loading').message="";
                    },1800)
                    return;
                }
                if(this.api.joinRequestQueue.length){
                    clearInterval(interval);
                    let pckg=this.api.joinRequestQueue.shift();
                    Alpine.store('confirmBox').show(
                        "Join Request",
                        `${pckg.nickname} wants to join!`
                    ).then(()=>{
                        this.api.acceptJoinRequest(pckg.nickname);
                        this.p1.name=pckg.nickname;
                        while(this.api.joinRequestQueue.length){
                            this.api.rejectJoinRequest(this.joinRequestQueue.shift().nickname);
                        }
                        Alpine.store('loading').message=`${pckg.nickname} Joining`
                    }).catch(()=>{
                        this.api.rejectJoinRequest(pckg.nickname);
                    }).finally(()=>{
                        this.waitJoin();
                    })
                }
            })
        },
        changeTurn(){
            if(this.args.status=="check"){
                if(this.playerColor==this.args.color){
                    Alpine.store('messageBox').show("Alert!","Check!");
                }else{
                    Alpine.store('messageBox').show("Alert!","You Check The Enemy!");
                }
            }
            if(this.args.status=="playing"||this.args.status=="check"){
                this.roundMessage=`Now Moving: ${this.args.color}'s turn`;
                this.nowMovingColor=this.args.color;
                if(this.isSingle) this.playerColor=this.args.color;
                Alpine.store('board').update();
            }else{
                this.stopUpdate();
                Alpine.store('onClose').remove();
                let title,message;
                if(this.args.status=="tie"){
                    title="Game Over";
                    message="The game tied";
                }else{
                    title="Congrats!!";
                    message=`${this.args.color} ${this.args.status}s`;
                }
                this.roundMessage=message;
                Alpine.store('messageBox').show(title,message).then(()=>{
                    location.href="/";
                })
            }
        },
        readHistory(choice){
            this.board = [
                '........',
                '........',
                '........',
                '........',
                '........',
                '........',
                '........',
                '........'
            ]
            setTimeout(()=>{
                this.api.readHistory(choice).then((resolve)=>{
                    Alpine.store('board').resetXY();
                    this.changeTurn(resolve.args);
                });
            },75)
        },
        resign(){
            Alpine.store('confirmBox').show(
                "Alert!",
                "Do you want to resign?"
            ).then(()=>{
                this.api.resign(this.playerColor);
            })
        }
    })
    Alpine.store('board',{
        board:[],
        maskBoard:[],
        update(){
            this.board=Alpine.store('game').args.board;
            this.maskBoard=Alpine.store('game').args.maskBoard;
        },
        clickedX:-1,
        clickedY:-1,
        maskChess(x,y) {
            this.board[y] =
              this.board[y].substring(0, x) +
              '.' + this.board[y].substring(x + 1);

            this.board[this.clickedY] =
              this.board[this.clickedY].substring(0, this.clickedX) +
              '.' + this.board[this.clickedY].substring(this.clickedX + 1);
        },
        resetXY(){
            this.clickedX=-1;
            this.clickedY=-1;
            this.maskBoard=[];
        },
        click(x,y){
            let clicked=this.clickedX!==-1&&this.clickedY!==-1;
            let clickSameColor=(
                Alpine.store('utility').isupper(this.board[y][x])
                && Alpine.store('game').nowMovingColor === "white"
            )||(
                Alpine.store('utility').islower(this.board[y][x])
                && Alpine.store('game').nowMovingColor === "black"
            )
            let clickDiff=this.clickedX!==x||this.clickedY!==y;
            if(!clicked||clickSameColor){
                if(this.clickedX===x&&this.clickedY===y) this.resetXY();
                else if(this.board[y][x]!=='.'&&clickSameColor){
                    this.clickedX=x;
                    this.clickedY=y;
                    Alpine.store('game').api.preview(x,y).then((resolve)=>{
                        this.board=resolve.args.board;
                        this.maskBoard=resolve.args.maskBoard;
                        this.maskBoard[y*8+x]=1;
                    })
                }
            }else{
                if(clickDiff){
                    Alpine.store('game').stopUpdate();
                    Alpine.store('game').api.move(
                        this.clickedX,
                        this.clickedY,
                        x,
                        y
                    ).then((resolve)=>{
                        if(resolve.args.value==="failed"){
                            if(Alpine.store('game').isSingle) Alpine.store('easterEgg').count++;
                            Alpine.store('messageBox').show(
                                "Alert!!",
                                `invalid move${
                                    Alpine.store('easterEgg').count>1
                                        ?` ${Alpine.store('easterEgg').count}/5`
                                        :''
                                }`
                            ).then(()=>{
                                if(Alpine.store('easterEgg').count>=5) Alpine.store('easterEgg').show();
                            })
                            Alpine.store('game').startUpdate();
                            return;
                        }else Alpine.store('easterEgg').count=0;
                        if(resolve.args.value==="success"){
                            this.maskChess(x,y);
                            this.resetXY();
                            Alpine.store('game').update(resolve);
                            Alpine.store('game').changeTurn();
                            Alpine.store('game').startUpdate();
                        }
                        if(resolve.args.value==="promotion"){
                            Alpine.store('music').pause('bgm');
                            Alpine.store('music').playFromHead('promoting');
                            Alpine.store('promotionBox').show().then((choice)=>{
                                Alpine.store('music').stop('promoting');
                                Alpine.store('music').play('promoted');
                                Alpine.store('game').api.promotion(choice).then((resolve)=>{
                                    this.maskChess(x,y);
                                    this.resetXY();
                                    Alpine.store('game').update(resolve);
                                    Alpine.store('game').changeTurn();
                                    Alpine.store('game').startUpdate();
                                })
                            })
                        }
                    })
                }else this.resetXY();
            }
        }
    })
    Alpine.store('easterEgg',{
        count:0,
        on:false,
        show(){
            Alpine.store('game').stopUpdate();
            Alpine.store('music').pauseAll();
            Alpine.store('messageBox').show("😂","😂😂😂😂😂");
            Alpine.store('music').play('rick');
            Alpine.store('board').board = [
                'zzzzzzzz',
                'zzzzzzzz',
                'zzzzzzzz',
                'zzzzzzzz',
                'zzzzzzzz',
                'zzzzzzzz',
                'zzzzzzzz',
                'zzzzzzzz'
            ]
            setInterval(()=>{
                this.on=false;
                this.on=true;
                Alpine.store('board').maskBoard=Alpine.store('board').maskBoard.map(()=>{return parseInt(Math.random()*2)})
            },1000)
        }
    })
    Alpine.store('onClose',{
        event(e){
            Alpine.store('game').stopUpdate();
            let title=e.code==1000?"Game Over":"Connection Closed";
            let message=e.code==1006?"Server is down":e.reason;
            Alpine.store('messageBox').show(title,message).then(()=>{
                location.href="/";
            })
        },
        add(){
            Alpine.store('game').api.ws.addEventListener('close',this.event.bind(this));
        },
        remove(){
            Alpine.store('game').api.ws.removeEventListener('close',this.event.bind(this));
        }
    })
    Alpine.store('loading',{
        on:true,
        message:"",
        dot:"...",
        start(){
            let count=0;
            setInterval(()=>{
                this.dot=".".repeat(count++);
                if(count>=4) count=0;
            },500)
        }
    })
    Alpine.store('messageBox',{
        on:false,
        title:"",
        content:"",
        show(title,message){
            this.title=title;
            this.content=message;
            document.querySelector('#messageBox').close();
            document.querySelector('#messageBox').showModal();
            this.on=true;
            return new Promise((resolve)=>{
                let interval=setInterval(()=>{
                    if(!this.on){
                        clearInterval(interval);
                        resolve();
                    }
                },100)
            })
        },
        close(){
            this.on=false;
            setTimeout(()=>{
                this.title="";
                this.content="";
                document.querySelector('#messageBox').close();
            },75)
        }
    })
    Alpine.store('confirmBox',{
        on:false,
        title:"",
        content:"",
        choice:false,
        show(title,message){
            this.title=title;
            this.content=message;
            document.querySelector('#confirmBox').close();
            document.querySelector('#confirmBox').showModal();
            this.on=true;
            return new Promise((resolve,reject)=>{
                let interval=setInterval(()=>{
                    if(!this.on){
                        clearInterval(interval);
                        if(this.choice) resolve();
                        reject();
                    }
                },100)
            })
        },
        close(bool){
            this.choice=bool;
            this.on=false;
            setTimeout(()=>{
                this.title="";
                this.content="";
                document.querySelector('#confirmBox').close();
            },75)
        }
    })
    Alpine.store('promotionBox',{
        on:false,
        choice:0,
        show(){
            document.querySelector('#promotionBox').close();
            document.querySelector('#promotionBox').showModal();
            this.on=true;
            return new Promise((resolve)=>{
                let interval=setInterval(()=>{
                    if(!this.on){
                        clearInterval(interval);
                        resolve(this.choice);
                    }
                },100)
            })
        },
        close(choice){
            this.choice=choice+1;
            this.on=false;
            setTimeout(()=>{
                this.title="";
                this.content="";
                document.querySelector('#promotionBox').close();
            },75)
        },
        calculateChess() {
            const options = ['q', 'b', 'n', 'r']
            const isBlack = Alpine.store('game').nowMovingColor === 'black'
            if (isBlack) return options;
            return options.map((char) => {
                return char.toUpperCase()
            })
        }
    })
    Alpine.store('music',{
        on:false,
        volume:50,
        updateVolume(){
            document.querySelectorAll('audio').forEach((audio)=>{
                audio.volume=this.volume/100;
            })
        },
        play(name){
            if(!this.on) return;
            document.querySelector(`audio#${name}`).play();
        },
        playFromHead(name){
            if(!this.on) return;
            document.querySelector(`audio#${name}`).load();
            document.querySelector(`audio#${name}`).play();
        },
        pause(name){
            if(!this.on) return;
            document.querySelector(`audio#${name}`).pause();
        },
        stop(name){
            if(!this.on) return;
            document.querySelector(`audio#${name}`).pause();
            document.querySelector(`audio#${name}`).load();
        },
        pauseAll(){
            document.querySelectorAll('audio').forEach((audio)=>audio.pause());
        }
    })

    Alpine.store('confirmBox').show(
        "Permission",
        "Play Music?"
    ).then(()=>{
        Alpine.store('music').on=true;
    }).finally(()=>{
        Alpine.store('game').connect();
    })
})