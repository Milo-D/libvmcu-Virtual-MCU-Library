/* Implementation of AVR Data Memory */

// C Headers
#include <stdlib.h>
#include <string.h>

// Project Headers (engine)
#include "engine/include/system/core/data.h"
#include "engine/include/system/core/io.h"

// Project Headers (engine utilities)
#include "engine/include/arch/mcudef.h"
#include "engine/include/misc/bitmanip.h"

/* --- Extern --- */

vmcu_data_t* vmcu_data_ctor(void) {

    vmcu_data_t *data;

    if((data = malloc(sizeof(vmcu_data_t))) == NULL)
        return NULL;  

    data->memory = malloc((RAM_END + 1) * sizeof(int8_t));
    memset(data->memory, 0x00, (RAM_END + 1) * sizeof(int8_t));

    data->io = vmcu_io_ctor(&data->memory[GPR_SIZE]);
    
    data->memory[SPL] = spl(RAM_END);
    data->memory[SPH] = sph(RAM_END);

    return data;
}

void vmcu_data_dtor(vmcu_data_t *this) {

    vmcu_io_dtor(this->io);

    free(this->memory);
    free(this);
}

void vmcu_data_push(vmcu_data_t *this, const int8_t value) {

    const uint8_t spl = this->memory[SPL];
    const uint8_t sph = this->memory[SPH];

    uint16_t sp = sp(spl, sph);

    if(sp > RAM_END)
        return;

    this->memory[sp--] = value;
    this->memory[SPL] = spl(sp);
    this->memory[SPH] = sph(sp);
}

int8_t vmcu_data_pop(const vmcu_data_t *this) {

    const uint8_t spl = this->memory[SPL];
    const uint8_t sph = this->memory[SPH];

    uint16_t sp = sp(spl, sph);

    if(sp >= RAM_END)
        return 0xff;

    const int8_t value = this->memory[++sp];

    this->memory[SPL] = spl(sp);
    this->memory[SPH] = sph(sp);

    return value;
}

void vmcu_data_write(vmcu_data_t *this, const uint16_t addr, const int8_t value) {

    if(addr > RAM_END)
        return;

    if(addr >= SFR_START && addr <= SFR_END) {

        vmcu_io_write_sfr(this->io, addr, value);
        return;
    }

    this->memory[addr] = value;
}

int8_t vmcu_data_read(const vmcu_data_t *this, const uint16_t addr) {

    if(addr > RAM_END)
        return 0xff;

    if(addr >= SFR_START && addr <= SFR_END)
        return vmcu_io_read_sfr(this->io, addr);

    return this->memory[addr];
}

int8_t* vmcu_data_dump(const vmcu_data_t *this) {

    return this->memory;
}

void vmcu_data_reboot(const vmcu_data_t *this) {

    vmcu_io_reboot(this->io);
    memset(this->memory, 0x00, (RAM_END + 1) * sizeof(int8_t));

    this->memory[SPL] = spl(RAM_END);
    this->memory[SPH] = sph(RAM_END);
}

void vmcu_data_update_io(const vmcu_data_t *this, const uint32_t cpu_clk, const uint64_t dc) {

    vmcu_io_update(this->io, cpu_clk, dc);
}

int vmcu_data_check_irq(const vmcu_data_t *this, uint32_t *isr) {

    return vmcu_io_check_irq(this->io, isr);
}

void vmcu_data_set_sfr(vmcu_data_t *this, const uint16_t addr, const int bit) {
    
    if(addr < SFRL_START || addr > SFRL_END)
        return;

    vmcu_io_set_sfr(this->io, addr, bit);
}

void vmcu_data_clear_sfr(vmcu_data_t *this, const uint16_t addr, const int bit) {
    
    if(addr < SFRL_START || addr > SFRL_END)
        return;

    vmcu_io_clear_sfr(this->io, addr, bit);
}

int8_t* vmcu_data_dump_eeprom(const vmcu_data_t *this) {
    
    return vmcu_io_dump_eeprom(this->io);
}
