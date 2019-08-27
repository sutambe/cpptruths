/**
 * Definition for singly-linked list.
 * function ListNode(val) {
 *     this.val = val;
 *     this.next = null;
 * }
 */

/**
 * @param {ListNode} head
 * @return {ListNode}
 */
var reverseList = function(head) {
    if (head == null) {
        return null;
    }
    if (head.next == null) {
        return head;
    }
    var prev = head;
    var node = head.next;
    head.next = null;
    while (node.next != null) {
        var next = node.next;
        node.next = prev;
        prev = node;
        node = next;
    }
    node.next = prev;
    return node;
};
