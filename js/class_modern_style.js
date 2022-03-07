// Classe + constructeur
class Shape {
    constructor(x, y) {
        this.move_to(x, y);
        this.fill_color = "white";
    }

    // Méthodes d'instance

    move_to(x, y) {
        this.x = x;
        this.y = y;
    }

    draw() {
        return `(${this.x},${this.y})`;
    }

    // Méthodes statiques

    static create_random_shape() {
        const p = Math.round(Math.random() * 1000);
        return new Shape(p, p);
    }
}

// Classe enfant spécialisant Shape
class Square extends Shape {
    constructor(x, y, size) {
        super(x, y);
        this.size = size;
    }

    draw() {
        const basis = super.draw();
        return `${basis} [side ${this.size}, filled in ${this.fill_color}]`;
    }
}

const shape = new Shape(42, 20);
shape.move_to(10, 10);
console.log(shape);
console.log(shape.draw());
console.log(Shape.create_random_shape());

const square = new Square(42, 42, 100);
console.log(square);
square.move_to(10, 10);
console.log(square.draw());
