function average_1() {
    const args = Array.from(arguments);
    const len = args.length;
    return args.reduce((acc, curr) => acc + curr) / len;
}

console.log(average_1(12, 7, 18, 9, 14, 18));

const average_2 = (...args) => {
    const len = args.length;
    return args.reduce((acc, curr) => acc + curr) / len;
};

console.log(average_2(12, 7, 18, 9, 14, 18));

const sources = new Set(["Alice", "Bob", "Charlie", "Dave", "Eve", "Frank"]);
// const sizes = [...sources].map(w => w.length);
const sizes = Array.from(sources, w => w.length);
console.log(sizes);
