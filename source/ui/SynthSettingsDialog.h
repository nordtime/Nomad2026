#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class SynthSettingsDialog : public juce::Component
{
public:
    struct Result
    {
        juce::String synthName = "Nord Modular";
        int midiChannel1 = 1;
        int midiChannel2 = 2;
        int midiChannel3 = 3;
        int midiChannel4 = 4;
        int globalChannel = 16;
        int clockSource = 0; // 0 = Internal, 1 = External
        int masterTune = 0; // cents
        int knobMode = 0; // 0 = Normal, 1 = Hook
        int pedalPolarity = 0; // 0 = Normal, 1 = Inverted
    };

    SynthSettingsDialog(const Result& initialSettings, std::function<void(const Result&)> onApply);
    
    void resized() override;
    void paint(juce::Graphics& g) override;

    static void show(juce::Component* parent, const Result& initialSettings, std::function<void(const Result&)> onApply);

private:
    Result currentSettings;
    std::function<void(const Result&)> onApplyCallback;

    juce::Label nameLabel { {}, "Synth Name:" };
    juce::TextEditor nameEditor;

    juce::Label midiCh1Label { {}, "Slot A MIDI Ch:" };
    juce::ComboBox midiCh1Combo;

    juce::Label midiCh2Label { {}, "Slot B MIDI Ch:" };
    juce::ComboBox midiCh2Combo;

    juce::Label midiCh3Label { {}, "Slot C MIDI Ch:" };
    juce::ComboBox midiCh3Combo;

    juce::Label midiCh4Label { {}, "Slot D MIDI Ch:" };
    juce::ComboBox midiCh4Combo;

    juce::Label globalChLabel { {}, "Global MIDI Ch:" };
    juce::ComboBox globalChCombo;

    juce::Label clockLabel { {}, "Clock Source:" };
    juce::ComboBox clockCombo;

    juce::Label tuneLabel { {}, "Master Tune (cents):" };
    juce::Slider tuneSlider;

    juce::Label knobModeLabel { {}, "Knob Mode:" };
    juce::ComboBox knobModeCombo;

    juce::Label pedalLabel { {}, "Pedal Polarity:" };
    juce::ComboBox pedalCombo;
    
    juce::TextButton okButton { "OK" };
    juce::TextButton cancelButton { "Cancel" };

    void saveAndClose();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthSettingsDialog)
};
