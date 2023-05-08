import "./api/Alpine.js";
import {Game} from "./api/Game.js"

document.addEventListener('alpine:init', () => {
    console.log('0.0');
    Alpine.data('globalScope', () => ({
        board: [
        'rnbqkbnr',
        'pppppppp',
        '........',
        '........',
        '........',
        '........',
        'PPPPPPPP',
        'RNBQKBNR'
        ],
        maskBoard:[],
        nowMoving: 'white',
        calculateUrl: (input)=>{
            if(input !== '.'){
                if(input === input.toLowerCase())
                    return './assets/b'+ input + '.svg';
                return './assets/w'+ input.toLowerCase() + '.svg';
            }
            return false;
        },
        clickedX:-1,
        clickedY:-1,
        click(x,y){
            if(this.clickedX==-1&&this.clickedY==-1){
                this.clickedX=x;
                this.clickedY=y;
                this.game.preview(x,y).then((resolve)=>{
                    this.maskBoard=resolve;
                }).catch(error=>console.error(error))
            }else{
                if(x!=this.clickedX||y!=this.clickedY)
                    this.game.move(
                        this.clickedX,
                        this.clickedY,
                        x,
                        y
                    ).then((resolve)=>{
                        if(resolve=="success"){
                            this.game.printBoard().then((resolve)=>{
                                this.board=resolve;
                            }).catch(error=>console.error(error))
                        }
                        if(resolve=="promotion"){

                        }
                    }).catch(error=>console.error(error))
                this.clickedX=-1;
                this.clickedY=-1;
            }
        },
        game:new Game()
    }))
})