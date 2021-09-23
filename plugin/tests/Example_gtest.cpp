#include "PluginProcessor.h"
#include <gtest/gtest.h>

/* This example test shows how to create an instance of the plugin as 
   part of a test. */
TEST(ExampleTest, CreatePluginAndGUITest)
{
    juce::ScopedJuceInitialiser_GUI juceInitializer;
    auto messageManager = juce::MessageManager::getInstance();
    messageManager->setCurrentThreadAsMessageThread();

    auto pluginInstance = std::make_unique<TapeLooperPluginAudioProcessor>();
    EXPECT_NE(pluginInstance, nullptr);
    auto gui = pluginInstance->createEditorIfNeeded();
    EXPECT_NE(gui, nullptr);

    messageManager->runDispatchLoopUntil(200);

    pluginInstance->editorBeingDeleted(gui);
    delete gui;

    pluginInstance.reset();
    messageManager->deleteInstance();
}