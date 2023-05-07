let ws=new WebSocket("ws://localhost:1234");

ws.onopen=()=>{
    console.log("web socket loaded!");
}

window.onload=()=>{
    let opt=document.querySelector('#opt');
    ws.onmessage=(e)=>{
        opt.innerHTML=e.data;
    }

    let ipt=document.querySelector('#ipt');
    let btn=document.querySelector('#btn');
    btn.addEventListener('click',()=>{
        ws.send(ipt.value);
    })
}