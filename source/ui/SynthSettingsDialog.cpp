#include "SynthSettingsDialog.h"

SynthSettingsDialog::SynthSettingsDialog(const Result& initialSettings, std::function<void(const Result&)> onApply)
    : currentSettings(initialSettings), onApplyCallback(onApply)
{
    setSize(400, 480);

    addAndMakeVisible(nameLabel);
    addAndMakeVisible(nameEditor);
    nameEditor.setText(currentSettings.synthName, juce::dontSendNotification);

    auto setupMidiCombo = [this](juce::ComboBox& combo, int initialVal) {
        addAndMakeVisible(combo);
        for (int i = 1; i <= 16; ++i)
            combo.addItem(juce::String(i), i);
        combo.setSelectedId(initialVal, juce::dontSendNotification);
    };

    addAndMakeVisible(midiCh1Label); setupMidiCombo(midiCh1Combo, currentSettings.midiChannel1);
    addAndMakeVisible(midiCh2Label); setupMidiCombo(midiCh2Combo, currentSettings.midiChannel2);
    addAndMakeVisible(midiCh3Label); setupMidiCombo(midiCh3Combo, currentSettings.midiChannel3);
    addAndMakeVisible(midiCh4Label); setupMidiCombo(midiCh4Combo, currentSettings.midiChannel4);
    addAndMakeVisible(globalChLabel); setupMidiCombo(globalChCombo, currentSettings.globalChannel);

    addAndMakeVisible(clockLabel);
    addAndMakeVisible(clockCombo);
    clockCombo.addItem("Internal", 1);
    clockCombo.addItem("External", 2);
    clockCombo.setSelectedId(currentSettings.clockSource == 0 ? 1 : 2, juce::dontSendNotification);

    addAndMakeVisible(tuneLabel);
    addAndMakeVisible(tuneSlider);
    tuneSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    tuneSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 60, 20);
    tuneSlider.setRange(-50.0, 50.0, 1.0);
    tuneSlider.setValue(currentSettings.masterTune, juce::dontSendNotification);
    tuneSlider.setTextValueSuffix(" ct");

    addAndMakeVisible(knobModeLabel);
    addAndMakeVisible(knobModeCombo);
    knobModeCombo.addItem("Normal", 1);
    knobModeCombo.addItem("Hook", 2);
    knobModeCombo.setSelectedId(currentSettings.knobMode == 0 ? 1 : 2, juce::dontSendNotification);

    addAndMakeVisible(pedalLabel);
    addAndMakeVisible(pedalCombo);
    pedalCombo.addItem("Normal", 1);
    pedalCombo.addItem("Inverted", 2);
    pedalCombo.setSelectedId(currentSettings.pedalPolarity == 0 ? 1 : 2, juce::dontSendNotification);

    addAndMakeVisible(okButton);
    okButton.onClick = [this]() { saveAndClose(); };

    addAndMakeVisible(cancelButton);
    cancelButton.onClick = [this]() {
        if (auto* dw = findParentComponentOfClass<juce::DialogWindow>())
            dw->exitModalState(0);
    };
}

void SynthSettingsDialog::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff2a2a3e));
}

void SynthSettingsDialog::resized()
{
    auto area = getLocalBounds().reduced(20);
    int labelWidth = 140;
    int rowHeight = 28;
    int spacing = 8;

    auto row = [&](juce::Component& label, juce::Component& ctrl) {
        auto r = area.removeFromTop(rowHeight);
        label.setBounds(r.removeFromLeft(labelWidth));
        ctrl.setBounds(r);
        area.removeFromTop(spacing);
    };

    row(nameLabel, nameEditor);
    area.removeFromTop(spacing);
    row(midiCh1Label, midiCh1Combo);
    row(midiCh2Label, midiCh2Combo);
    row(midiCh3Label, midiCh3Combo);
    row(midiCh4Label, midiCh4Combo);
    row(globalChLabel, globalChCombo);
    area.removeFromTop(spacing);
    row(clockLabel, clockCombo);
    row(tuneLabel, tuneSlider);
    row(knobModeLabel, knobModeCombo);
    row(pedalLabel, pedalCombo);

    auto btnArea = area.removeFromBottom(30);
    cancelButton.setBounds(btnArea.removeFromRight(100));
    btnArea.removeFromRight(10);
    okButton.setBounds(btnArea.removeFromRight(100));
}

void SynthSettingsDialog::saveAndClose()
{
    Result res;
    res.synthName = nameEditor.getText();
    res.midiChannel1 = midiCh1Combo.getSelectedId();
    res.midiChannel2 = midiCh2Combo.getSelectedId();
    res.midiChannel3 = midiCh3Combo.getSelectedId();
    res.midiChannel4 = midiCh4Combo.getSelectedId();
    res.globalChannel = globalChCombo.getSelectedId();
    res.clockSource = clockCombo.getSelectedId() == 1 ? 0 : 1;
    res.masterTune = static_cast<int>(tuneSlider.getValue());
    res.knobMode = knobModeCombo.getSelectedId() == 1 ? 0 : 1;
    res.pedalPolarity = pedalCombo.getSelectedId() == 1 ? 0 : 1;

    if (onApplyCallback)
        onApplyCallback(res);

    if (auto* dw = findParentComponentOfClass<juce::DialogWindow>())
        dw->exitModalState(1);
}

void SynthSettingsDialog::show(juce::Component* parent, const Result& initialSettings, std::function<void(const Result&)> onApply)
{
    auto* dialog = new SynthSettingsDialog(initialSettings, onApply);

    juce::DialogWindow::LaunchOptions opts;
    opts.content.setOwned(dialog);
    opts.dialogTitle = "Synth Settings";
    opts.dialogBackgroundColour = juce::Colour(0xff2a2a3e);
    opts.escapeKeyTriggersCloseButton = true;
    opts.useNativeTitleBar = true;
    opts.resizable = false;
    opts.launchAsync();
}
