import {Socket} from "./Socket.js"

/**
 * @typedef {Object} Player
 * @property {string} name
 * @property {string} ip
 */

/**
 * @typedef {Object} Point
 * @property {int} x
 * @property {int} y
 */

export class PlayChess{
    /**
     * Create a game with two player
     * @param {Player} p0 
     * @param {Player} p1 
     * @returns {int} gameId
     */
    constructor(p0,p1){
        /** @type {Player} */
        this.player0=p0;
        /** @type {Player} */
        this.player1=p1;
        /** @type {Player} */
        this.nowPlayer=p0;
    }
}