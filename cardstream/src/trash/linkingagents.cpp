Agent* table[N_MAX_TABLE]; unsigned int nActiveAgentsLinked; // pointer of Agents who are playing

void Shoe::LinkAgent(Agent* newAgent) {
    table[nActiveAgentsLinked++] = newAgent;
}