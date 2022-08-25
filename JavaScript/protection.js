// ancien style
function Person(first, last) {
    this.first = first;
    this.last = last;
}

// Aucune protection : le mot-clé new est omis
const person = Person("John", "Doe"); //! mauvais
//const person = new Person('John', 'Doe'); //! ok
// Conséquences :
console.log(
    "first n'a pas été défini dans la portée globale, et pourtant: ",
    first
);
console.log(
    "last n'a pas été défini dans la portée globale, et pourtant: ",
    last
);

// moderne style
class ESPerson {
    constructor(first, last) {
        this.first = first;
        this.last = last;
    }
}

// Protection : le mot-clé new DOIT être présent.
//const esperson = ESPerson('John', 'Doe'); //! erreur
const esperson = new ESPerson("John", "Doe"); //! ok

// Protection : super DOIT être présent lors de l'héritage.
class Geek extends Person {
    constructor(first, last, nick) {
        super(first, last);
        this.nick = nick;
    }
}
