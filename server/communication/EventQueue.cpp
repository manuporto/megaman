#include <vector>
#include <queue>

#include "EventQueue.h"

void EventQueue::flush_previous_actions() {
    for (unsigned int i = 0; i < this->action_queues.size(); ++i) {
        while (!this->action_queues[i]->is_empty())
            delete this->action_queues[i]->pop();
    }
}

EventQueue::EventQueue(const std::vector<PacketsQueueProtected*>& action_queues)
        : action_queues(action_queues), quit(false) {
    this->flush_previous_actions();
    this->start();
}

void EventQueue::run() {
    while (!this->quit) {
        for (unsigned int i = 0; i < this->action_queues.size(); ++i) {
            Action *packet;
            if (!this->action_queues[i]->is_empty()) {
                packet = (Action *) this->action_queues[i]->pop();
                Lock l(this->m);
                this->queue.push(packet);
            }
        }
    }
}

bool EventQueue::is_empty() {
    Lock l(this->m);
    return this->queue.empty();
}

Action* EventQueue::pop() {
    Lock l(this->m);
    Action* action = this->queue.front();
    this->queue.pop();
    return action;
}

void EventQueue::shutdown() {
    this->quit = true;
}

EventQueue::~EventQueue() {
    this->shutdown();
    this->join();
}
