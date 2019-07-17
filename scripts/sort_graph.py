import argparse
import tensorflow as tf
from tensorflow.tools.graph_transforms import TransformGraph

parser = argparse.ArgumentParser(description='Sort inference graph')
parser.add_argument('--input', required=True, help='Path to frozen TensorFlow graph.')
parser.add_argument('--output', required=True, help='Path to output.')
args = parser.parse_args()

with tf.gfile.FastGFile(args.input, 'rb') as f:
    graph_def = tf.GraphDef()
    graph_def.ParseFromString(f.read())
    graph_def = TransformGraph(graph_def, ['image_tensor'], ['detection_boxes', 'detection_classes', 'detection_scores', 'num_detections'], ['sort_by_execution_order'])
    with tf.gfile.FastGFile(args.output, 'wb') as f:
        f.write(graph_def.SerializeToString())