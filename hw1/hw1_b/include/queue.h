#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <common/types.h>

namespace myos
{
    // Node structure for the priority queue
    struct Node
    {
        int data;
        int priority;
        Node *next;
    };

    // Priority Queue class
    class PriorityQueue
    {
    public:
        // Constructor
        PriorityQueue();

        // Destructor
        ~PriorityQueue();

        // Check if the priority queue is empty
        bool isEmpty();

        // Insert an element with a given priority
        void enqueue(int data, int priority);

        // Remove and return the element with the highest priority
        int dequeue();

        // Get the element with the highest priority without removing it
        int peek();

    private:
        Node *head; // Pointer to the head of the linked list
    };
}

#endif // PRIORITY_QUEUE_H