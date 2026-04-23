#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_data_structures/juce_data_structures.h>

class EditorOptionsDialog : public juce::Component
{
public:
    EditorOptionsDialog(juce::ApplicationProperties& props, std::function<void()> onSettingsChanged);

    void resized() override;
    void paint(juce::Graphics& g) override;

    static void show(juce::ApplicationProperties& props, std::function<void()> onSettingsChanged);

private:
    juce::ApplicationProperties& appProperties;
    std::function<void()> onSettingsChangedCallback;

    juce::Label cableStyleLabel;
    juce::ComboBox cableStyleCombo;
    juce::Label knobControlLabel;
    juce::ComboBox knobControlCombo;
    
    juce::ToggleButton autoUploadButton { "Auto Upload Parameter Changes" };
    juce::ToggleButton recycleWindowsButton { "Recycle Patch Windows" };

    void loadSettings();
    void saveSettings();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EditorOptionsDialog)
};
