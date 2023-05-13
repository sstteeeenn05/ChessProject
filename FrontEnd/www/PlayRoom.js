import "./api/Alpine.js";
import {Game} from "./api/Game.js"

document.addEventListener('alpine:init', () => {
    Alpine.data('globalScope', () => ({
        isupper(input){
            return (input === input.toUpperCase() && input !== '.');
        },
        islower(input){
            return (input === input.toLowerCase() && input !== '.');
        },
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
        isPromoting: false,
        previewClass: [
            '',
            'indianred'
        ],
        nowMoving: 'white',
        calculateUrl(input) {
            if (input !== '.') {
                if (this.islower(input))
                    return './assets/b' + input + '.svg';
                return './assets/w' + input.toLowerCase() + '.svg';
            }
            return './assets/nothing.png';
        },
        calculatePromotionChess() {
            const options = ['q', 'b', 'n', 'r']
            const isBlack = this.nowMoving === 'black'
            if (isBlack) return options;
            return options.map((char) => {
                return char.toUpperCase()
            })
        },
        showResult(player, status) {
            if (status !== 'tie') {
                this.showMessage('Congrats!!', `${player} ${status}s`)
            } else {
                this.showMessage('Game Over', 'the game tied')
            }

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

        },
        changeTurn({status, who, canUndo, canRedo}) {
            this.canUndo = canUndo
            this.canRedo = canRedo
            if (status === 'check') alert('check')
            if (status === 'tie') {
                this.showResult(this.nowMoving, 'tie')
            } else if (status === 'win') {
                this.showResult(who, status);
            } else {
                this.nowMoving = who
            }
        },
        canUndo: false,
        canRedo: false,
        readHistory(option) {
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
                this.game.readHistory(option).then(() => {
                    this.game.getBoard().then((resolve) => {
                        this.board = resolve.value
                        this.resetXY()
                        this.changeTurn(resolve)
                    })
                })
            },30)

        },
        promotion(option) {
            this.game.promotion(option + 1).then(() => {
                this.maskChess()
                this.game.getBoard().then(
                  (resolve) => {
                      this.board = resolve.value
                      this.resetXY()
                      this.isPromoting = false;
                      setTimeout(() => {
                          document.getElementById('promotion').close()
                          this.changeTurn(resolve)
                      }, 75)

                  }
                )
            })
        },
        maskChess() {
            this.board[this.clickingY] =
              this.board[this.clickingY].substring(0, this.clickingX) +
              '.' + this.board[this.clickingY].substring(this.clickingX + 1);

            this.board[this.clickedY] =
              this.board[this.clickedY].substring(0, this.clickedX) +
              '.' + this.board[this.clickedY].substring(this.clickedX + 1);
        },
        resetXY() {
            this.clickedX = -1;
            this.clickedY = -1;
            this.clickingX = -1;
            this.clickingY = -1;
            this.maskBoard = [];
        },
        clickedX: -1,
        clickedY: -1,
        clickingX: -1,
        clickingY: -1,
        click(x, y) {
            if ((this.clickedX === -1 && this.clickedY === -1) ||
                (this.isupper(this.board[y][x]) && this.nowMoving === 'white') ||
                (this.islower(this.board[y][x]) && this.nowMoving === 'black')) {

                if(this.clickedX === x && this.clickedY === y) {
                    this.resetXY()
                }

                else if(this.board[y][x] !== '.' &&
                    (this.isupper(this.board[y][x]) && this.nowMoving === 'white') ||
                    (this.islower(this.board[y][x]) && this.nowMoving === 'black')){
                    this.clickedX = x;
                    this.clickedY = y;
                    this.game.preview(x, y).then((resolve) => {
                        this.maskBoard = resolve.value;
                        this.maskBoard[y*8+x] = 1;
                    })
                }
            } else {

                if (x !== this.clickedX || y !== this.clickedY) {
                    this.game.move(
                      this.clickedX,
                      this.clickedY,
                      x,
                      y
                    ).then(
                      (resolve) => {
                          if (resolve.value === 'failed') {
                              this.showMessage('Alert!!', 'invalid move')
                              return;
                          }
                          this.clickingX = x;
                          this.clickingY = y;
                          if (resolve.value === "success") {
                              this.maskChess()
                              this.game.getBoard().then((resolve) => {
                                  this.board = resolve.value;
                                  this.resetXY()
                                  this.changeTurn(resolve)
                              })
                          }
                          if (resolve.value === "promotion") {
                              let lightbox = document.getElementById('promotion')
                              lightbox.showModal();
                              this.isPromoting = true
                          }
                      }
                    )
                } else this.resetXY()

            }
        },
        game: new Game(),
        roomId: "",
        connectGame(){
            this.game.connect(
                "create",
                "127.0.0.1",
                "abc",
                true
            ).then((resolve)=>{
                this.roomId=resolve;
                setTimeout(()=>this.loading=false,1800);
            })
        }
    }))
})