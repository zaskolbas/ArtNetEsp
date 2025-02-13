#include "strobe.h"

Strobe::Strobe(uint8_t universe, uint8_t channel, uint8_t pin, int pulse, int multiplier, int activeState)
{
    LOG("New Strobe: pin=" + String(pin) + " DMX channel:" + String(channel));

    this->universe = universe;
    this->channel = channel;
    this->pin = pin;
    this->pulse = pulse;
    this->multiplier = multiplier;
    enabled = true;
    this->activeState = activeState;
    this->inactiveState = activeState == HIGH ? LOW : HIGH;
    this->state = activeState;
    this->valueOverride = activeState == HIGH ? 0 : 255;
    this->adjustedMaxValue = activeState == HIGH ? 255 : 0;

    interval = 0;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, inactiveState);
}

uint8_t Strobe::get(uint8_t channel)
{
    return value;
}

void Strobe::set(uint8_t dmxChannel, uint8_t data)
{
    if (dmxChannel == channel) // Dimmer
    {
        LOG("Dimmer: " + String(dmxChannel) + " = " + String(data));
        this->value = data;
        this->adjustedActiveValue = activeState == HIGH ? value : 255 - value;
        this->adjustedInactiveValue = activeState == HIGH ? 0 : 255;
        update();
    }
    else if (dmxChannel == channel - 1) // Strobe
    {
        LOG("Dimmer Strobe: " + String(dmxChannel) + " = " + String(data));
        setDuration(pulse);
        setInterval(data * multiplier);
    }
}

void Strobe::update()
{
    if (enabled && state == activeState)
    {
        analogWrite(pin, adjustedActiveValue);
        LOG(" =" + String(adjustedActiveValue));
    }
    else
    {
        analogWrite(pin, adjustedInactiveValue);
    }
}

void Strobe::setInterval(int millis)
{
    period = millis;
    if (period < 0)
        period = pulse;
}

void Strobe::setDuration(int millis)
{
    pulse = millis;
    if (pulse < 0)
        pulse = 0;
}

void Strobe::setPin(int number)
{
    if (pin != number)
    {
        pinMode(pin, INPUT);
        pin = number;
        pinMode(pin, OUTPUT);
        digitalWrite(pin, inactiveState);
    }
}

void Strobe::handle()
{
    if (!enabled)
        return;
    unsigned long currentMillis = millis();
    if (lastChange != 0 && millis() - lastChange > DMX_SILENCE_TIMEOUT)
    {
        LOG(F("DMX Silence Timeout"));
        set(channel, 0);
        lastChange = millis();
    }
    else if (period <= pulse && enabled)
    {
        previousMillis = currentMillis;
        if (state != activeState)
        {
            state = activeState;
            update();
        }
    }
    else
    {
        if (currentMillis - previousMillis >= interval && enabled)
        {
            previousMillis = currentMillis;
            if (state == activeState)
            {
                state = inactiveState;
                interval = period - pulse;
            }
            else
            {
                state = activeState;
                interval = pulse;
            }
            update();
        }
    }
}

void Strobe::start()
{
    if (!enabled)
    {
        LOG(F("Strobe Started"));
        interval = 0;
        state = activeState;
        enabled = true;
        lastChange = millis();
    }
}

void Strobe::stop()
{
    enabled = false;
    digitalWrite(pin, inactiveState);
}

void Strobe::flip()
{
    enabled = !enabled;
    if (enabled)
    {
        LOG(F("Flip - ON"));
        // flipping when DMX value is set to 0. Manual flip should bring the full light on
        valueOverride = value > 0 ? adjustedActiveValue : adjustedMaxValue;
    }
    else
    {
        LOG(F("Flip - OFF"));
        valueOverride = adjustedInactiveValue;
    }
    update();
}

bool Strobe::isEnabled()
{
    return enabled;
}