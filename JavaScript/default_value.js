// @ts-nocheck

// < ES2015
function default_old(term, times, separator) {
    times = times || 1;
    separator = separator || "-";
    return Array(times).fill(term).join(separator);
}

// Problème:
console.log("old", JSON.stringify(default_old("a", 0))); // -> 0 vaudra 1

// >= ES2015
function default_modern(term, times = 1, separator = "-") {
    return Array(times).fill(term).join(separator);
}

console.log("modern", JSON.stringify(default_modern("b", 0))); // -> 0 vaudra bien 0
console.log("modern", JSON.stringify(default_modern("c", 2, "%")));

function banner(title, banner = "-".repeat(title.length)) {
    return [banner, title, banner].join("\n");
}

console.log(banner("Hello"));
console.log(banner("?????", "*".repeat(42)));

function required(name = is_required("name")) {
    return name;
}

function is_required(arg) {
    return new Error("Paramètre manquant: " + arg + " est requis.");
}

console.log(required()) // throw Error
console.log(required("Bob")) // Bob
