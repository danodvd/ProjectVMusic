#include "Command.hpp"

struct QueueItemView;

class DeleteQueueCommand : public Command {
public:
	DeleteQueueCommand(AudioManager& audioRef, std::vector<QueueItemView>& items) : audio(audioRef), items(items) {}

    void Execute() override {
        audio.Stop();
        audio.ClearQueue();

        items.clear();
    }
private:
    AudioManager& audio;
    std::vector<QueueItemView>& items;
};