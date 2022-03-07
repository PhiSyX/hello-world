class PasswordManager {
    static #STORAGE_KEY = Symbol("pm:storage-key");

    #key;

    constructor(key) {
        this.#key = key;
    }

    add_identity(ident) {
        const store = this.#open();
        store.add(ident);
        store.persist();
    }

    #open() {
        /* code */
        return CryptoStorage.get(PasswordManager.#STORAGE_KEY).open(this.#key);
    }
}

class CryptoStorage {
    static get(key) {
        /* code */
        return new CryptoStorage();
    }

    add(ident) {
        /* code */
    }
    persist() {
        /* code */
    }

    open(key) {
        /* code */
        return this;
    }
}

const pm = new PasswordManager("my-key");
console.log(Object.keys(pm));
