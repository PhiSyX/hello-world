mod contention_measure;
mod help;
mod normalized_lockfree;
mod operation_record;
mod waitfree_simulator;

/*


head -> A (@0x1)

insert B (@0x2); B.next = 0x1
CAS(head, 0x1, 0x2)
  -> success if A is still at the head
  -> fail if A is no longer at the head
    because B.next would need to be different

meanwhile..

insert C (@0x3); C.next = 0x1
CAS(head, 0x1, 0x3)
remove A
CAS(C.next, 0x1, 0x0)
insert D (@0x1); D.next = 0x3
CAS(head, 0x3, 0x1)
head -> D(@0x1) -> C(@0x3) -> .

and now..
insert of B continues.

CAS(head, 0x1, 0x2)
  -> should have failed

head -> B(@0x2) -> A(@0x1) actually D(@0x1) -> C(@0x3) -> .


*/

/*
// in a consuming crate (wait-free-linked-list crate)
struct WaitFreeLinkedList<T> {
    simulator: WaitFreeSimulator<LockFreeLinkedList<T>>,
}

struct LockFreeLinkedList<T> {
    t: T,
}

impl<T> NormalizedLockFree for LockFreeLinkedList<T> {}

impl<T> WaitFreeLinkedList<T> {
    pub fn push_front(&self, T: T) {
        self.simulator.run(Insert(i))
    }
}
 */
