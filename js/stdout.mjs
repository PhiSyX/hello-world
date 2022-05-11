import { stdout } from "node:process";

const TIMER = 1000;
const INTERVAL = 100;
let currentTextPosition = 3;

function* chars(input) {
    for (const ch of input) {
        yield ch;
    }
}

function prints_hello_world() {
    const text = "Hello, World  !";
    const hello_world = chars("Hello, World    !");

    stdout.cursorTo(currentTextPosition);

    setInterval(function () {
        const next_ch = hello_world.next();

        if (next_ch.done) {
            clearInterval(this);
            currentTextPosition = text.length - 6;
            prints_phisyx();
            return;
        }

        ++currentTextPosition;

        stdout.write(next_ch.value);
    }, INTERVAL);
}

function prints_phisyx() {
    const name = chars("PhiSyX ;)");

    stdout.cursorTo(++currentTextPosition);

    setInterval(function () {
        const next_ch = name.next();

        if (next_ch.done) {
            clearInterval(this);
            return;
        }

        ++currentTextPosition;

        stdout.write(next_ch.value);
    }, INTERVAL);
}

async function prints_timer() {
    function timer(ms) {
        return {
            async *[Symbol.asyncIterator]() {
                let i = 0;
                while (true) {
                    await new Promise((resolve) => setTimeout(resolve, ms));
                    yield ++i;
                }
            }
        }
    }

    stdout.cursorTo(0);
    stdout.write("0: ");

    for await (const i of timer(TIMER)) {
        stdout.cursorTo(0);
        stdout.write(i.toFixed());
        stdout.write(": ");
        stdout.cursorTo(currentTextPosition);
        if (i == 5) {
            stdout.write("\r\nFini!\r\n");
            break;
        }
    }
}

prints_timer();
prints_hello_world();
