#include "HetrickCV.hpp"
#include "dsp/digital.hpp"

struct Boolean3 : Module 
{
	enum ParamIds 
	{
		NUM_PARAMS
	};
	enum InputIds 
	{
        INA_INPUT,
        INB_INPUT,
        INC_INPUT,
		NUM_INPUTS
	};
	enum OutputIds 
	{
        OR_OUTPUT,
        AND_OUTPUT,
        XOR_OUTPUT,
        NOR_OUTPUT,
        NAND_OUTPUT,
        XNOR_OUTPUT,
		NUM_OUTPUTS
    };
    
    enum LightIds 
    {
        OR_LIGHT,
        AND_LIGHT,
        XOR_LIGHT,
        NOR_LIGHT,
        NAND_LIGHT,
        XNOR_LIGHT,
		INA_LIGHT,
        INB_LIGHT,
        INC_LIGHT,
        NUM_LIGHTS
	};

    bool inA = false;
    bool inB = false;
    bool inC = false;
    float outs[6] = {};

	Boolean3() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) 
	{
		
	}

	void step() override;

	// For more advanced Module features, read Rack's engine.hpp header file
	// - toJson, fromJson: serialization of internal data
	// - onSampleRateChange: event triggered by a change of sample rate
	// - reset, randomize: implements special behavior when user clicks these from the context menu
};


void Boolean3::step() 
{
    inA = (inputs[INA_INPUT].value >= 1.0f);
    inB = (inputs[INB_INPUT].value >= 1.0f);
    inC = (inputs[INC_INPUT].value >= 1.0f);

    lights[INA_LIGHT].value = inA ? 5.0f : 0.0f;
    lights[INB_LIGHT].value = inB ? 5.0f : 0.0f;
    lights[INC_LIGHT].value = inC ? 5.0f : 0.0f;

    outs[0] = ((inA || inB) || inC) ? 5.0f : 0.0f;
    outs[1] = ((inA && inB) && inC) ? 5.0f : 0.0f;
    outs[2] = (!inA && (inB ^ inC)) || (inA && !(inB || inC)) ? 5.0f : 0.0f;
    outs[3] = 5.0f - outs[0];
    outs[4] = 5.0f - outs[1];
    outs[5] = 5.0f - outs[2];

    outputs[OR_OUTPUT].value = outs[0];
    outputs[AND_OUTPUT].value = outs[1];
    outputs[XOR_OUTPUT].value = outs[2];
    outputs[NOR_OUTPUT].value = outs[3];
    outputs[NAND_OUTPUT].value = outs[4];
    outputs[XNOR_OUTPUT].value = outs[5];

    lights[OR_LIGHT].value = outs[0];
    lights[AND_LIGHT].value = outs[1];
    lights[XOR_LIGHT].value = outs[2];
    lights[NOR_LIGHT].value = outs[3];
    lights[NAND_LIGHT].value = outs[4];
    lights[XNOR_LIGHT].value = outs[5];
}


Boolean3Widget::Boolean3Widget() 
{
	auto *module = new Boolean3();
	setModule(module);
	box.size = Vec(6 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

	{
		auto *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/Boolean3.svg")));
		addChild(panel);
	}

	addChild(createScrew<ScrewSilver>(Vec(15, 0)));
	addChild(createScrew<ScrewSilver>(Vec(box.size.x - 30, 0)));
	addChild(createScrew<ScrewSilver>(Vec(15, 365)));
	addChild(createScrew<ScrewSilver>(Vec(box.size.x - 30, 365)));

    //////PARAMS//////

    //////INPUTS//////
    addInput(createInput<PJ301MPort>(Vec(10, 105), module, Boolean3::INA_INPUT));
    addInput(createInput<PJ301MPort>(Vec(10, 195), module, Boolean3::INB_INPUT));
    addInput(createInput<PJ301MPort>(Vec(10, 285), module, Boolean3::INC_INPUT));

    //////OUTPUTS//////
    addOutput(createOutput<PJ301MPort>(Vec(45, 60), module, Boolean3::OR_OUTPUT));
    addOutput(createOutput<PJ301MPort>(Vec(45, 105), module, Boolean3::AND_OUTPUT));
    addOutput(createOutput<PJ301MPort>(Vec(45, 150), module, Boolean3::XOR_OUTPUT));
    addOutput(createOutput<PJ301MPort>(Vec(45, 195), module, Boolean3::NOR_OUTPUT));
    addOutput(createOutput<PJ301MPort>(Vec(45, 240), module, Boolean3::NAND_OUTPUT));
    addOutput(createOutput<PJ301MPort>(Vec(45, 285), module, Boolean3::XNOR_OUTPUT));

    //////BLINKENLIGHTS//////
    addChild(createLight<SmallLight<RedLight>>(Vec(74, 68), module, Boolean3::OR_LIGHT));
    addChild(createLight<SmallLight<RedLight>>(Vec(74, 113), module, Boolean3::AND_LIGHT));
    addChild(createLight<SmallLight<RedLight>>(Vec(74, 158), module, Boolean3::XOR_LIGHT));
    addChild(createLight<SmallLight<RedLight>>(Vec(74, 203), module, Boolean3::NOR_LIGHT));
    addChild(createLight<SmallLight<RedLight>>(Vec(74, 248), module, Boolean3::NAND_LIGHT));
    addChild(createLight<SmallLight<RedLight>>(Vec(74, 293), module, Boolean3::XNOR_LIGHT));
    addChild(createLight<SmallLight<RedLight>>(Vec(18, 92), module, Boolean3::INA_LIGHT));
    addChild(createLight<SmallLight<RedLight>>(Vec(18, 182), module, Boolean3::INB_LIGHT));
    addChild(createLight<SmallLight<RedLight>>(Vec(18, 272), module, Boolean3::INC_LIGHT));
}