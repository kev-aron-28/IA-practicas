#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <limits.h>

// Definición de constantes para el tamaño del lienzo, la cuadrícula y las imágenes
#define CANVAS_SIZE 280
#define GRID_SIZE 28

#define IMAGE_SIZE 784
#define NUM_CLASSES 10

// Estructura para representar una imagen y su etiqueta
typedef struct
{
  unsigned char pixels[IMAGE_SIZE]; // Píxeles de la imagen en escala de grises
  unsigned char label;              // Etiqueta asociada a la imagen
} Image;

TTF_Font *font = NULL; // Fuente para renderizar texto

// Función para convertir enteros de little-endian a big-endian
int reverse_integer(int i)
{
  unsigned char c1, c2, c3, c4;
  c1 = i & 255;
  c2 = (i >> 8) & 255;
  c3 = (i >> 16) & 255;
  c4 = (i >> 24) & 255;

  return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
}

// Función para cargar imágenes de prueba desde un archivo
Image *load_test_images(const char *filename, int count)
{
  FILE *file = fopen(filename, "rb");
  if (!file)
  {
    perror("Error opening the file \n");
    exit(1);
  }

  int magic_number, num_images, num_rows, num_columns;

  // Leer y verificar el número mágico del archivo
  fread(&magic_number, sizeof(int), 1, file);
  magic_number = reverse_integer(magic_number);
  if (magic_number != 2051)
  {
    fprintf(stderr, "Invalid magic number: %d\n", magic_number);
    fclose(file);
    exit(1);
  }

  // Leer metadatos del archivo
  fread(&num_images, sizeof(int), 1, file);
  fread(&num_rows, sizeof(int), 1, file);
  fread(&num_columns, sizeof(int), 1, file);

  num_images = reverse_integer(num_images);
  num_rows = reverse_integer(num_rows);
  num_columns = reverse_integer(num_columns);

  // Reservar memoria para las imágenes
  Image *images = (Image *)malloc(count * sizeof(Image));
  for (int i = 0; i < count; i++)
  {
    fread(images[i].pixels, sizeof(unsigned char), IMAGE_SIZE, file);
    images[i].label = 0; // Inicializar etiquetas a 0
  }

  fclose(file);
  return images;
}

// Función para cargar etiquetas de prueba desde un archivo
Image *load_test_labels(const char *filename, Image *images, int count)
{
  FILE *file = fopen(filename, "rb");
  if (!file)
  {
    perror("Error opening file \n");
    exit(1);
  }

  int magic_number, num_labels;

  // Leer y verificar el número mágico del archivo
  fread(&magic_number, sizeof(int), 1, file);
  magic_number = reverse_integer(magic_number);

  fread(&num_labels, sizeof(int), 1, file);

  // Leer las etiquetas y asociarlas a las imágenes
  for (int i = 0; i < count; i++)
  {
    fread(&images[i].label, sizeof(unsigned char), 1, file);
  }

  fclose(file);
}

// Función para calcular la distancia euclidiana entre dos vectores
double euclidean_distance(unsigned char *a, unsigned char *b, int size)
{
  double distance = 0.0;
  for (int i = 0; i < size; i++)
  {
    double diff = (double)a[i] - (double)b[i];
    distance += diff * diff;
  }
  return sqrt(distance);
}

// Implementación del algoritmo k-NN para clasificar imágenes
unsigned char knn_classify(Image *training_images, int train_count, unsigned char *test_image, int k)
{
  double distances[train_count]; // Distancias entre la imagen de prueba y las de entrenamiento
  int indices[train_count];     // Índices de las imágenes de entrenamiento

  // Calcular distancias
  for (int i = 0; i < train_count; i++)
  {
    distances[i] = euclidean_distance(test_image, training_images[i].pixels, IMAGE_SIZE);
    indices[i] = i;
  }

  // Ordenar las distancias de menor a mayor (ordenamiento burbuja)
  for (int i = 0; i < train_count - 1; i++)
  {
    for (int j = i + 1; j < train_count; j++)
    {
      if (distances[i] > distances[j])
      {
        double temp_distance = distances[i];
        distances[i] = distances[j];
        distances[j] = temp_distance;

        int temp_indx = indices[i];
        indices[i] = indices[j];
        indices[j] = temp_indx;
      }
    }
  }

  int label_counts[NUM_CLASSES] = {0}; // Conteo de etiquetas en los k vecinos más cercanos

  // Incrementar el conteo para las etiquetas de los k vecinos más cercanos
  for (int i = 0; i < k; i++)
  {
    int label = training_images[indices[i]].label;
    label_counts[label]++;
  }

  // Determinar la etiqueta más frecuente entre los vecinos
  int max_count = 0, best_label = 0;
  for (int i = 0; i < NUM_CLASSES; i++)
  {
    if (label_counts[i] > max_count)
    {
      max_count = label_counts[i];
      best_label = i;
    }
  }

  return best_label;
}

// Apartir de aqui son puras cosas de SLD2 PINPLANER  

void render_text(SDL_Renderer *renderer, const char *text, int x, int y)
{

  SDL_Color white = {255, 255, 255, 255};
  SDL_Surface *surface = TTF_RenderText_Solid(font, text, white);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  int text_width = surface->w;
  int text_height = surface->h;
  SDL_Rect dst_rect = {x, y, text_width, text_height};

  SDL_RenderCopy(renderer, texture, NULL, &dst_rect);

  SDL_DestroyTexture(texture);
  SDL_FreeSurface(surface);
}

void draw_image(SDL_Renderer *renderer, unsigned char *pixels, int x_offset, int y_offset)
{
  for (int i = 0; i < GRID_SIZE; i++)
  {
    for (int j = 0; j < GRID_SIZE; j++)
    {
      unsigned char pixel = pixels[i * GRID_SIZE + j];
      SDL_SetRenderDrawColor(renderer, pixel, pixel, pixel, 255); // Escala de grises
      SDL_RenderDrawPoint(renderer, x_offset + j, y_offset + i);
    }
  }
}

// Aqui es donde probamos las imagenes de prueba con las de entrenamiento
void display_result(SDL_Renderer *renderer, Image *test_images, Image *train_images, int train_count, int test_count, int window_width, int window_height)
{
  SDL_Event event;

  int x_offset = (window_width / 2) - (CANVAS_SIZE / 2);
  int y_offset = (window_height / 2) - (CANVAS_SIZE / 2);

  while (SDL_PollEvent(&event))
  {
    if (event.type == SDL_QUIT)
    {
      return; // Salir del bucle y finalizar
    }
  }

  int correct_predictions = 0;

  for (int i = 0; i < test_count; i++)
  {
    unsigned char *test_image = test_images[i].pixels;
    unsigned char predicted_label = knn_classify(train_images, train_count, test_image, 3);

    // Cambiar el color del fondo según la predicción
    if (predicted_label == test_images[i].label)
    {
      SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
      correct_predictions++;
    }
    else
    {
      SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    }

    SDL_RenderClear(renderer);

    draw_image(renderer, test_image, x_offset, y_offset);

    char test_number_text[50];
    snprintf(test_number_text, sizeof(test_number_text), "Test #%d: Predicted %d", i + 1, predicted_label);
    render_text(renderer, test_number_text, x_offset, y_offset + CANVAS_SIZE + 5); // Texto debajo de la imagen

    SDL_RenderPresent(renderer);

    SDL_Delay(1000);
  }

  float accuracy = ((float)correct_predictions / test_count) * 100.0;

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  char accuracy_text[100];
  snprintf(accuracy_text, sizeof(accuracy_text), "Accuracy: %.2f%%", accuracy);
  render_text(renderer, accuracy_text, x_offset, y_offset); // Centrado en pantalla

  SDL_RenderPresent(renderer);
  SDL_Delay(1000);
}

int main()
{
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window *window = SDL_CreateWindow("KNN Visualización", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, SDL_WINDOW_SHOWN);

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  if (TTF_Init() == -1)
  {
    fprintf(stderr, "SDL_ttf Error: %s\n", TTF_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  font = TTF_OpenFont("arial.ttf", 24);
  if (!font)
  {
    printf("Error al cargar la fuente: %s\n", TTF_GetError());
    return -1;
  }

  // Numero de pruebas que realiza
  int train_count = 10;

  int test_count = train_count;

  const char *train_images_file = "train-images.idx3-ubyte";

  const char *train_labels_file = "train-labels.idx1-ubyte";

  Image *train_images = load_test_images(train_images_file, train_count);

  load_test_labels(train_labels_file, train_images, train_count);

  const char *test_images_file = "t10k-images.idx3-ubyte";

  const char *test_labels_file = "t10k-labels.idx1-ubyte";

  Image *test_images = load_test_images(test_images_file, train_count);

  load_test_labels(test_labels_file, test_images, train_count);

  display_result(renderer, test_images, train_images, train_count, test_count, CANVAS_SIZE, CANVAS_SIZE);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
