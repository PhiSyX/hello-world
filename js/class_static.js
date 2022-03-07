class Translator {
    static translations = {
        yes: "si",
        no: "no",
        maybe: "può darsi",
    };

    static english_words = [];
    static italian_words = [];

    static {
        for (const [e, i] of Object.entries(this.translations)) {
            this.english_words.push(e);
            this.italian_words.push(i);
        }
    }
}

console.log(Translator);
