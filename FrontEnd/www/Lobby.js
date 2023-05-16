import "./api/Alpine.js";
const WS_URL="ws://localhost:1234";

document.addEventListener('alpine:init',()=>{
    Alpine.data('globalScope',()=>{
        return{
            showMode:"home",
            roomList:[],
            gettingRoomList:false,
            getRoomList(){
                this.gettingRoomList=true;
                new Promise((resolve,reject)=>{
                    let ws = new WebSocket(WS_URL,"protocol-chess-game");
                    ws.onopen=()=>{
                        ws.onmessage=(e)=>{
                            ws.onclose=null;
                            ws.close();
                            let pckg=JSON.parse(e.data.toString());
                            if(pckg.type!="room-list") reject();
                            resolve(pckg.content);
                        }
                        ws.onerror=()=>reject();
                        ws.onclose=()=>reject();
                        ws.send(JSON.stringify({
                            type:"handshake",
                            content:{header:"getRoomList"}
                        }))
                    }
                }).then((resolve)=>{
                    setTimeout(()=>{
                        this.roomList=resolve;
                        setTimeout(()=>{
                            this.gettingRoomList=false;
                        },500);
                    },1000);
                })
            },
            joinRoom(id){
                let nickname=prompt('Enter your nickname');
                if(nickname) location.href=`PlayRoom.html?join=${id}&nickname=${nickname}`;
            },
            isCreatePromptOpen:false,
            createPromptRoomName:"",
            createPromptNickName:"",
            showCreatePrompt(){
                this.isCreatePromptOpen=true;
                document.getElementById('createPromptBox').showModal()
            },
            closeCreatePrompt(value){
                if(value){
                    let roomId=this.createPromptRoomName;
                    let nickname=this.createPromptNickName;
                    if(!roomId) return this.showMessage("Alert!","Missing Room Name");
                    if(!nickname) nickname="Unknown";
                    location.href=`PlayRoom.html?create=${roomId}&nickname=${nickname}`;
                }
                this.isCreatePromptOpen=false;
                setTimeout(()=>{
                    this.createPromptRoomName="";
                    this.createPromptNickName="";
                    document.getElementById('createPromptBox').close()
                })
            },
            isJoinPromptOpen:false,
            joinPromptRoomName:"",
            joinPromptNickName:"",
            showJoinPrompt(id){
                this.isJoinPromptOpen=true;
                this.joinPromptRoomName=id;
                document.getElementById('joinPromptBox').showModal()
            },
            closeJoinPrompt(value){
                if(value){
                    let roomId=this.joinPromptRoomName;
                    let nickname=this.joinPromptNickName;
                    if(!nickname) nickname="Unknown";
                    location.href=`PlayRoom.html?join=${roomId}&nickname=${nickname}`;
                }
                this.isJoinPromptOpen=false;
                setTimeout(()=>{
                    this.joinPromptRoomName="";
                    this.joinPromptNickName="";
                    document.getElementById('joinPromptBox').close()
                },75)
            },
            isMessage: false,
            boxTitle: '',
            boxContext: '',
            showMessage(title, message) {
                this.boxTitle = title
                this.boxContext = message
                document.getElementById('messageBox').showModal()
                this.isMessage = true
            },
            closeMessage() {
                this.isMessage = false;
                setTimeout(() => {
                    this.boxTitle = ''
                    this.boxContext = ''
                    document.getElementById('messageBox').close()
                }, 75)
    
            }
        }
    })
})