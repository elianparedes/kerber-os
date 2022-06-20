#ifndef _MODULELOADER_H
#define _MODULELOADER_H

/**
 * @brief Loads given modules into memory
 * 
 * @param payloadStart payload start address
 * @param moduleTargetAddress modules to load to
 */
void loadModules(void *payloadStart, void **moduleTargetAddress);

#endif /* _MODULELOADER_H */