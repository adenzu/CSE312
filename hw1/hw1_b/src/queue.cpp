#include <queue.h>

myos::PriorityQueue::PriorityQueue()
{
}

myos::PriorityQueue::~PriorityQueue()
{
    while (!isEmpty())
    {
        dequeue();
    }
}

// Check if the priority queue is empty
bool myos::PriorityQueue::isEmpty()
{
    return head == nullptr;
}

// Insert an element with a given priority
void myos::PriorityQueue::enqueue(int data, int priority)
{
    Node *newNode = new Node;
    newNode->data = data;
    newNode->priority = priority;

    // If the queue is empty or the new node has higher priority
    if (isEmpty() || priority > head->priority)
    {
        newNode->next = head;
        head = newNode;
    }
    else
    {
        Node *current = head;
        while (current->next != nullptr && current->next->priority >= priority)
        {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
}

// Remove and return the element with the highest priority
int myos::PriorityQueue::dequeue()
{
    if (isEmpty())
    {
        return -1;
    }

    Node *temp = head;
    int data = head->data;
    head = head->next;
    // delete temp;
    return data;
}

// Get the element with the highest priority without removing it
int myos::PriorityQueue::peek()
{
    if (isEmpty())
    {
        return -1;
    }

    return head->data;
}
