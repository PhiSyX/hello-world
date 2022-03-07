function register_player_1(name, avatar, marker_prop_name) {
    const result = { name: name, avatar: avatar };
    result[marker_prop_name] = 42;
    return result;
}

console.log(register_player_1("John", "regular-dude", "__jx87pm_player__"));

function register_player_2(name, avatar, marker_prop_name) {
    return { name, avatar, [marker_prop_name]: 42 };
}

console.log(register_player_2("John", "regular-dude", "__jx87pm_player__"));

function register_player_3(name, avatar, marker_prop_name) {
    return {
        name,
        avatar,
        [marker_prop_name]() {
            const base = [Date.now().toString(16), avatar, name].join("-");
            return base.toLowerCase().replace(/[^\w\d-]+/g, "");
        },
    };
}

console.log(register_player_3("John", "regular-dude", "ident").ident());

function make_dictionary(text) {
    const chars = text.toLowerCase().replace(/\P{Letter}+/gu, "");
    const unique_chars = new Set(Array.from(chars).sort());
    return {
        text,
        [Symbol.iterator]() {
            return unique_chars.values();
        },
    };
}

console.log(Array.from(make_dictionary("Hello, world!")));

function make_metronome(ms = 1000) {
    return {
        async *[Symbol.asyncIterator]() {
            while (true) {
                await new Promise((resolve) => setTimeout(resolve, ms));
                yield Date.now();
            }
        },
    };
}

const time_format = new Intl.DateTimeFormat("fr-FR", { timeStyle: "medium" });

async function demo() {
    for await (const stamp of make_metronome()) {
        console.log(time_format.format(new Date(stamp)));
    }
}

demo();
