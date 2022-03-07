// Classe + constructeur
function Shape(x, y) {
    this.move_to(x, y);
    this.fill_color = "white";
}

// Classe enfant spécialisant Shape
function Square(x, y, size) {
    // Appel au constructeur hérité
    Shape.call(this, x, y);
    this.size = size;
}

// Méthodes d'instance
Shape.prototype.move_to = function (x, y) {
    this.x = x;
    this.y = y;
};

Shape.prototype.draw = function () {
    return `(${this.x},${this.y})`;
};

// Méthodes statiques
Shape.create_random_shape = function () {
    const p = Math.round(Math.random() * 1000);
    return new Shape(p, p);
};

Square.prototype.draw = function () {
    const basis = Shape.prototype.draw.call(this);
    return `${basis} [side ${this.size}, filled in ${this.fill_color}]`;
};

function inherit(Child, Parent) {
    function SyntheticConstructor() {}
    SyntheticConstructor.prototype = Parent.prototype;

    Child.prototype = new SyntheticConstructor();
    Child.prototype.constructor = Child;
}

inherit(Square, Shape);

const shape = new Shape(42, 20);
shape.move_to(10, 10);
console.log(shape);
console.log(shape.draw());
console.log(Shape.create_random_shape());

const square = new Square(42, 42, 100);
console.log(square);
square.move_to(10, 10);
console.log(square.draw());
