type Iter<T> = {
	[Symbol.iterator](): Iterator<T>
};
