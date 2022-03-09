const obj = {
    name: "John Doe",
    first: "John",
    last: "Doe",
    age: 30,
    city: "New York",
};

function destructuring({
    my_object,
    my_object: { name, age }
}) {
    console.log(my_object, name, age);
}

destructuring({ my_object: obj });
