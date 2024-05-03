

const idle_state = {
    start: 0,
    end: 100,

    transition_gaps: [100], 

    change_to: [
        // { probability: 0.01, state: attention_1_state },
    ],
}

const attention_2_state = {
    start: 43,
    end: 56,

    transition_gaps: [56],

    change_to: [
        // { probability: 0.5, state: idle_state },
    ],
}

const attention_1_state = {
    start: 24,
    end: 42,

    transition_gaps: [42],

    change_to: [
        { transition_gap: 42, probability: 0.4, state: attention_2_state, at_state_end_start_from: 42 },
    ],
}



class Clippy {

    static current_state = attention_1_state;
    static animation_index = this.current_state.start;
    static state_queue = [];

    static nextFrame() {
        this.animation_index += 1;
        if (this.current_state.transition_gaps.includes(this.animation_index)) {
            if (this.state_queue > 0) {
                this.current_state = this.state_queue.shift();
                this.animation_index = this.current_state.start;
                return;
            }
            if (this.current_state.change_to.length > 0) {
                for (let i = 0; i < this.current_state.change_to.length; ++i) {
                    if (Math.random() < this.current_state.change_to[i].probability) {
                        this.state_queue.unshift(this.current_state);
                        this.current_state = this.current_state.change_to[i].state;
                        this.animation_index = this.current_state.start;
                        return;
                    }
                }
            }
        }

        if (this.animation_index > this.current_state.end) {
            if (this.state_queue.length > 0) {
                this.current_state = this.state_queue.shift();
                this.animation_index = this.current_state.start;
                return;
            }
            else {
                this.animation_index = this.current_state.start;
            }
        }
    }

    static setState(state) {
        this.state_queue.push(state);
    }


}
  



var clippy_canvas;
var clippy_canvas_ctx;

var clippy_spritesheet;


const clippy_count_x = 22;
const clippy_count_y = 41;

var clippy_w;
var clippy_h;
var clippy_aspect;

document.addEventListener("DOMContentLoaded", () => {

    clippy_canvas = document.getElementById("clippy");

    clippy_canvas_ctx = clippy_canvas.getContext("2d");


    clippy_spritesheet = new Image();
    clippy_spritesheet.src = "clippy/clippy.png";

    clippy_spritesheet.onload = () => {

        clippy_w = (clippy_spritesheet.width-1) / clippy_count_x;
        clippy_h = clippy_spritesheet.height / clippy_count_y;
        clippy_aspect = clippy_w / clippy_h;

        console.log("clippy:", clippy_w, clippy_h, clippy_aspect);

        return setInterval(clippy_draw, 80);

    };


});



function clippy_draw() {

    Clippy.nextFrame();

    clippy_canvas_ctx.clearRect(0, 0, clippy_canvas.width, clippy_canvas.height);

    clippy_canvas_ctx.drawImage(
        clippy_spritesheet, 
        clippy_w * (Clippy.animation_index % clippy_count_x),
        clippy_h * (Math.floor(Clippy.animation_index / clippy_count_x) % clippy_count_y),
        clippy_w, clippy_h, 0, 0, clippy_canvas.width, clippy_canvas.height);


} 