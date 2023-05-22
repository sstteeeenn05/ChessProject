import "./api/Alpine.js"

document.addEventListener('alpine:init',()=>{
    Alpine.store('test',{
        text:"abcdefg"
    })
    console.log(Alpine.store('test').text);
})