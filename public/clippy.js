

function has_value(arr, field_name, value) {
    for (let i = 0; i < arr.length; ++i) {
        if (arr[i][field_name] === value) return true;
    }
    return false;
}

const idle_state = {
    start: 0,
    end: 0,
 

    change_to: [
        // { probability: 0.01, state: attention_1_state },
    ],
}

const attention_2_state = {
    start: 43,
    end: 56,


    change_to: [
        // { probability: 0.5, state: idle_state },
    ],
}

const attention_1_state = {
    start: 24,
    end: 42,


    change_to: [
        { transition_gap: 42, probability: 1, state: attention_2_state, at_state_end_start_from: 42 },
    ],
}



class Clippy {

    static current_state = attention_1_state;
    static animation_index = this.current_state.start;
    static state_queue = [];

    static nextFrame() {
        this.animation_index += 1;
        


        for (let i = 0; i < this.current_state.change_to.length; ++i) if (this.current_state.change_to[i].transition_gap === this.animation_index) {

            if (this.current_state.change_to[i].probability >= 1 || 
                (Math.random() < this.current_state.change_to[i].probability &&
                this.state_queue.length == 0)
            ) {
                this.addStateUnshift(this.current_state, this.current_state.change_to[i].at_state_end_start_from);
                this.setState(this.current_state.change_to[i].state);
                console.log(this.animation_index, this.state_queue.length, this.current_state.start);
                return;
            }
        }

        if (this.animation_index > this.current_state.end) {
            if (this.state_queue.length > 0) {
                let qe = this.state_queue.shift();
                this.setState(qe.state, qe.start);
                console.log(this.animation_index, this.state_queue.length, this.current_state.start);
                return;
            }
            else {
                this.animation_index = this.current_state.start;
            }
        }

        console.log(this.animation_index, this.state_queue.length, this.current_state.start);
    }

    static setState(state, start = undefined) {
        this.state = state;
        this.animation_index = (typeof start === "number") ? start : state.start;
    }

    static addStateUnshift(state, start = undefined) {
        this.state_queue.unshift({state: state, start: (typeof start === "number") ? start : state.start});
    }
    static addState(state, start = undefined) {
        this.state_queue.push({state: state, start: (typeof start === "number") ? start : state.start});
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

        return setInterval(clippy_draw, 200);

    };


});



function clippy_draw() {

    

    clippy_canvas_ctx.clearRect(0, 0, clippy_canvas.width, clippy_canvas.height);

    clippy_canvas_ctx.drawImage(
        clippy_spritesheet, 
        clippy_w * (Clippy.animation_index % clippy_count_x),
        clippy_h * (Math.floor(Clippy.animation_index / clippy_count_x) % clippy_count_y),
        clippy_w, clippy_h, 0, 0, clippy_canvas.width, clippy_canvas.height
    );

    Clippy.nextFrame();
} 