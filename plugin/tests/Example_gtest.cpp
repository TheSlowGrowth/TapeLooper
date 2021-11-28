/**	
 * Copyright (C) Johannes Elliesen, 2021
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *  
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

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