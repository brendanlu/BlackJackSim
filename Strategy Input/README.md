# STRATEGY INPUT FORMAT
Use the TEMPLATE.csv format, and fill in the cells. 
Ensure that, if you use an editor like MS Excel, that you 'save as' csv file type, not xlsx. 

Currently, there is support for the following BJ player actions:
    'H': Hit
    'D': Double
    'R' Surrender
    'S': Stand
    'P': Split

The simulator enables you to turn off certain actions; you can provide an alternative to the first preferenced action by appending another action character afterwards. If the first is unable to be executed, the simulator will use the second action. If neither, the simulator will exist with a warning. 