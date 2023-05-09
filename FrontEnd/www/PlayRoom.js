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
        maskBoard: [],
        previewClass: [
            '',
            'indianred'
        ],
        nowMoving: 'white',
        calculateUrl: (input) => {
            if (input !== '.') {
                if (input === input.toLowerCase())
                    return './assets/b' + input + '.svg';
                return './assets/w' + input.toLowerCase() + '.svg';
            }
            return false;
        },
        calculatePromotionUrl() {
            const options = ['q', 'b', 'n', 'r']
            const isBlack = this.nowMoving[0] === 'b'
            if (isBlack) return options;
            return options.map((char) => {
                return char.toUpperCase()
            })
        },
        changeTurn(){
            this.game.getGameState().then(
                (resolve)=>{
                    resolve=resolve.split(' ')
                    if(resolve[0]==='black'){
                        this.nowMoving='black'
                    }
                    else if(resolve[0]==='white'){
                        this.nowMoving='white'
                    }
                    else {
                        tie();
                    }
                }
            ).catch(error=>console.log(error))
        },
        promotion(option) {
            this.game.promotion(option + 1).then((resolve) => {
                this.game.getBoard().then(
                    (data) => {
                        this.board = data
                        document.getElementById('promotion').close()
                    }
                ).catch(
                    (error) => console.log(error)
                )
            }).catch(error => console.error(error))
        },
        clickedX: -1,
        clickedY: -1,
        click(x, y) {
            if (this.clickedX == -1 && this.clickedY == -1) {
                this.clickedX = x;
                this.clickedY = y;
                this.game.preview(x, y).then((resolve) => {
                    this.maskBoard = resolve;
                }).catch(error => console.error(error))
            } else {
                if (x != this.clickedX || y != this.clickedY)
                    this.game.move(
                        this.clickedX,
                        this.clickedY,
                        x,
                        y
                    ).then((resolve) => {
                        if (resolve == "success") {
                            this.game.getBoard().then((resolve) => {
                                this.board = resolve;
                            }).catch(error => console.error(error))
                        }
                        if (resolve == "promotion") {
                            let lightbox = document.getElementById('promotion')
                            lightbox.showModal();
                        }
                    }).catch(error => console.error(error))
                this.clickedX = -1;
                this.clickedY = -1;
                this.maskBoard = []
            }
        },
        game: new Game()
    }))
})