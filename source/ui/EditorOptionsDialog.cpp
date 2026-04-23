#include "EditorOptionsDialog.h"

EditorOptionsDialog::EditorOptionsDialog(juce::ApplicationProperties& props, std::function<void()> onSettingsChanged)
    : appProperties(props), onSettingsChangedCallback(onSettingsChanged)
{
    setSize(300, 200);

    addAndMakeVisible(cableStyleLabel);
    cableStyleLabel.setText("Cable Style:", juce::dontSendNotification);
    
    addAndMakeVisible(cableStyleCombo);
    cableStyleCombo.addItem("Straight 3D", 1);
    cableStyleCombo.addItem("Curved 3D", 2);
    cableStyleCombo.addItem("Straight Thin", 3);
    cableStyleCombo.addItem("Curved Thin", 4);
    cableStyleCombo.onChange = [this]() { saveSettings(); };

    addAndMakeVisible(knobControlLabel);
    knobControlLabel.setText("Knob Control:", juce::dontSendNotification);
    
    addAndMakeVisible(knobControlCombo);
    knobControlCombo.addItem("Circular", 1);
    knobControlCombo.addItem("Horizontal", 2);
    knobControlCombo.addItem("Vertical", 3);
    knobControlCombo.onChange = [this]() { saveSettings(); };

    addAndMakeVisible(autoUploadButton);
    autoUploadButton.onClick = [this]() { saveSettings(); };

    addAndMakeVisible(recycleWindowsButton);
    recycleWindowsButton.onClick = [this]() { saveSettings(); };

    loadSettings();
}

void EditorOptionsDialog::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff2a2a3e));
}

void EditorOptionsDialog::resized()
{
    auto area = getLocalBounds().reduced(20);
    int rowHeight = 30;
    int labelWidth = 100;

    auto row1 = area.removeFromTop(rowHeight);
    cableStyleLabel.setBounds(row1.removeFromLeft(labelWidth));
    cableStyleCombo.setBounds(row1);
    area.removeFromTop(10); // spacing

    auto row2 = area.removeFromTop(rowHeight);
    knobControlLabel.setBounds(row2.removeFromLeft(labelWidth));
    knobControlCombo.setBounds(row2);
    area.removeFromTop(10); // spacing

    autoUploadButton.setBounds(area.removeFromTop(rowHeight));
    recycleWindowsButton.setBounds(area.removeFromTop(rowHeight));
}

void EditorOptionsDialog::loadSettings()
{
    if (auto* p = appProperties.getUserSettings())
    {
        // 1-based indices for combo box items (1=Straight3D, 2=Curved3D, etc)
        cableStyleCombo.setSelectedId(p->getIntValue("CableStyle", 2), juce::dontSendNotification);
        knobControlCombo.setSelectedId(p->getIntValue("KnobControl", 3), juce::dontSendNotification);
        autoUploadButton.setToggleState(p->getBoolValue("AutoUpload", false), juce::dontSendNotification);
        recycleWindowsButton.setToggleState(p->getBoolValue("RecycleWindows", false), juce::dontSendNotification);
    }
}

void EditorOptionsDialog::saveSettings()
{
    if (auto* p = appProperties.getUserSettings())
    {
        p->setValue("CableStyle", cableStyleCombo.getSelectedId());
        p->setValue("KnobControl", knobControlCombo.getSelectedId());
        p->setValue("AutoUpload", autoUploadButton.getToggleState());
        p->setValue("RecycleWindows", recycleWindowsButton.getToggleState());
        appProperties.saveIfNeeded();

        if (onSettingsChangedCallback)
            onSettingsChangedCallback();
    }
}

void EditorOptionsDialog::show(juce::ApplicationProperties& props, std::function<void()> onSettingsChanged)
{
    auto* dialog = new EditorOptionsDialog(props, onSettingsChanged);

    juce::DialogWindow::LaunchOptions opts;
    opts.content.setOwned(dialog);
    opts.dialogTitle = "Editor Options";
    opts.dialogBackgroundColour = juce::Colour(0xff2a2a3e);
    opts.escapeKeyTriggersCloseButton = true;
    opts.useNativeTitleBar = true;
    opts.resizable = false;
    opts.launchAsync();
}
