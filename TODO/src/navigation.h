
/**
 * @brief Função que encapsula a logica de navegação para passar pela tarefa do Slalom.
 * 
 * @details A função sera chamada logo após o AUV passar pelo gate e alinhar com o caminho.
 *          Ela ira procurar os objetos do Slalom e executar as ações necessárias para completar a tarefa.
 * *        A função é chamada dentro da classe AUVStateMachine e não possui argumentos nem retorno.
 *          A função deve chamar a função checksTransition() ao final para verificar se a transição de estado é necessária.
 *          A fução defineAction() da classe Thrusters deve ser usada para definir as ações do AUV.
 *          A partir dos objetos yoloCtrl e sensors é possivel obter dados dos objetos detectados e dados de sensores
 */
void AUVStateMachine::navigate();

/**
 * @brief Função que encapsula a logica de navegação para passar pela tarefa de DropMarkers.
 * 
 * @details A função sera chamada logo após o AUV passar pelo Slalom e alinhar com o caminho.
 *          Ela vai procurar pela caixa onde os marcadores devem ser derrubados e centralizar o marcador com a escolha inicial do AUV no gate
 *          Importante: Não é so centralizar a imagem na camera, é necessario levar em consideração onde o marcador esta localizado no AUV
 *          A função é chamada dentro da classe AUVStateMachine e não possui argumentos nem retorno.
 *          A função deve chamar a função checksTransition() ao final para verificar se a transição de estado é necessária.
 */
void AUVStateMachine::dropMarkers();

/**
 * @brief Função que encapsula a logica de navegação para passar pela tarefa de Tagging.
 * 
 * @details A função sera chamada logo após o AUV passar pelo DropMarkers e alinhar com o caminho.
 *          O AUV deve centralizar o objeto alvo na imagem, se aproximar o maximo possivel sem bater e disparar o torpedo. 
 *          A função é chamada dentro da classe AUVStateMachine e não possui argumentos nem retorno.
 *          A função deve chamar a função checksTransition() ao final para verificar se a transição de estado é necessária.
 */
void AUVStateMachine::tagging();

/**
 * @brief Função que encapsula a logica de navegação para passar pela tarefa de Cleanup.
 * 
 * @details A função sera chamada logo após o AUV passar pelo Tagging e alinhar com o caminho.
 *          Esta função é dependente do hidrofone para localizar o pinger
 */
void AUVStateMachine::cleanup();

/**
 * @brief Função que encapsula a logica de navegação para passar pela tarefa de retornar ao ponto inicial.
 * 
 * @details A função sera chamada logo após o AUV passar pelo Cleanup. 
 *          O AUV deve usar a localização atual e a localização inicial para retornar ao ponto inicial.
 *          A função é chamada dentro da classe AUVStateMachine e não possui argumentos nem retorno.
 *          Ao final o AUV deve subir à superfície e aguardar o resgate.
 */
void AUVStateMachine::returning();