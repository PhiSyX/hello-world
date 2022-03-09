const bob = {
    firstName: "Bob",
    scores: [10, 20],
    say_hello() {
        console.log(`Hello, je suis ${this.firstName}`);
    },
};

bob.say_hello(); // ok: "Hello, je suis Bob"

// "this" ne vaut pas l'objet `bob`...

setTimeout(bob.say_hello, 1); // erreur: "Hello, je suis undefined"
bob.scores.forEach(bob.say_hello); // erreur: "Hello, je suis undefined"

const c = bob.say_hello;
c(); // erreur: "Hello, je suis undefined"
c.call(bob); // ok: "Hello, je suis Bob"

const alice = {
    firstName: "Alice",
    scores: [30, 40],
    say_hello: bob.say_hello,
};

alice.say_hello(); // ok: "Hello, je suis Alice"

const fake = {
    0: "VB",
    1: "mort",
    2: "À",
    length: 3,
};

const methods = Array.prototype;
methods.reverse.call(fake);
const text = methods.join.call(fake, " ");
console.log(text); // ok: "À mort VB"
methods.push.apply(fake, ["et", "COBOL"]);
console.log(fake); // ok: { '0': 'À', '1': 'mort', '2': 'VB','3': 'et', '4': 'COBOL', length: 5 }
