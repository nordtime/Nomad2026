#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_core/juce_core.h>
#include <vector>
#include <string>

class PatchBrowserPanel : public juce::Component
{
public:
    PatchBrowserPanel();

    void paint(juce::Graphics& g) override;
    void resized() override;

    // Update the patch list from the synth
    void setPatchList(const std::vector<std::string>& names);
    void setLoadingState(bool loading);

    // Mark which patch is currently loaded in the editor (section/position, -1 = none)
    void setLoadedPatch(int section, int position);

    int getLoadedSection() const { return loadedSection; }
    int getLoadedPosition() const { return loadedPosition; }

    // Callbacks
    std::function<void(int section, int position)> onPatchDoubleClicked;
    std::function<void()> onRefreshRequested;
    std::function<void(int section, int position)> onPatchRename;
    std::function<void(int section, int position)> onPatchDelete;
    std::function<void(int section, int position)> onPatchCopy;
    std::function<void(int section, int position)> onPatchMove;
    std::function<void(const juce::File& file)> onSnippetDoubleClicked;

private:
    class PatchTreeItem : public juce::TreeViewItem
    {
    public:
        PatchTreeItem(const juce::String& name, int section = -1, int position = -1, PatchBrowserPanel* parent = nullptr, const juce::String& snippetPath = {});

        bool mightContainSubItems() override;
        juce::var getDragSourceDescription() override;
        void paintItem(juce::Graphics& g, int width, int height) override;
        void itemClicked(const juce::MouseEvent& e) override;
        void itemDoubleClicked(const juce::MouseEvent& e) override;

        void showContextMenu();

    private:
        juce::String itemName;
        int section;   // -1 for root/bank nodes
        int position;  // -1 for root/bank nodes
        PatchBrowserPanel* panel;
        juce::String snippetPath;
    };

    std::unique_ptr<juce::TreeView> treeView;
    std::unique_ptr<PatchTreeItem> rootItem;
    juce::Label statusLabel;
    bool isLoading = false;

    // Search and filter controls
    juce::Label searchLabel;
    juce::TextEditor searchBox;
    juce::ToggleButton hideEmptyButton;
    juce::TextButton refreshButton;

    // Cached patch list
    std::vector<std::string> cachedPatchList;
    juce::Array<juce::File> snippetFiles;
    juce::String currentSearchText;
    bool hideEmptySlots = false;

    // Currently loaded patch tracking
    int loadedSection = -1;
    int loadedPosition = -1;

    juce::File getSnippetDirectory() const;
    void scanSnippetFiles();

    void rebuildTree(const std::vector<std::string>& names);
    void applyFilters();
    void onSearchTextChanged();
    void onHideEmptyToggled();
    void onRefreshClicked();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PatchBrowserPanel)
};
